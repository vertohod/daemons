#include <boost/regex.hpp>
#include <unistd.h>
#include <list>

#include "defines_for_filter.h"
#include "processor.h"
#include "configure.h"
#include "alphabet.h"
#include "util.h"
#include "log.h"

// OBJECTS

#include "changed_nickname.h"
#include "password_hash.h"
#include "channel_user.h"
#include "found_user.h"
#include "channel.h"
#include "message.h"
#include "answer.h"
#include "status.h"
#include "client.h"
#include "inside.h"
#include "user.h"

namespace chatd
{

processor::processor(server_t& server) : m_server(server), m_stop_flag(false), m_data_flag(false)
{
    std::unordered_map<std::string, std::string> upstreams;

    m_bl = std::make_shared<swarm::swarm>(cfg().server_id, cfg().main, cfg().listen_base, cfg().secret_key, upstreams);
    // создаем таблицы
    m_bl->add_table<user>();
    m_bl->add_table<message>();
    m_bl->add_table<channel>();
    m_bl->add_table<channel_user>();
}

processor::~processor()
{
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_stop_flag = true;
        m_cv.notify_all();
    }
    for (auto& thread : m_threads) thread->join();
}

void processor::start(int count_threads)
{
    lo::l(lo::DEBUG) << "Start " << count_threads << " threads of processor";
    m_stop_flag = false;
    for (int count = 0; count < count_threads; ++count) {
        m_threads.push_back(std::make_shared<std::thread>(std::bind(&processor::thread_function, this)));
    }
}

void processor::thread_function()
{
    while (!m_stop_flag) {
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_cv.wait(lock, [this]{ return m_data_flag || m_stop_flag; });
        }
        // FIXME не понятно почему здесь такой while
        while (true) {
            swarm::connection hdl;
            std::shared_ptr<swarm::object> object_ptr; {
                std::lock_guard<std::mutex> lock(m_mutex);
                if (m_objects.empty()) break;
                hdl = m_objects.front().first;
                object_ptr = m_objects.front().second;
                m_objects.pop_front();
                m_data_flag = false;
            }
            process_commands(hdl, object_ptr);
        }
    }
}

void processor::add_command(swarm::connection hdl, std::shared_ptr<swarm::object>& object_ptr)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_objects.push_back(std::make_pair(hdl, object_ptr));
    m_data_flag = true;
    m_cv.notify_all();
}

void processor::process_commands(swarm::connection hdl, std::shared_ptr<swarm::object>& object_ptr)
{
    auto response_ptr = swarm::response::create();

    OID user_oid = 0;
    try {
        user_oid = m_connections.get_oid(hdl);
    } catch (const std::exception& e) {
        lo::l(lo::DEBUG) << e.what();
    }

    try {
        if (object_ptr->tp() == event::stp()) {

            auto event_ptr = std::dynamic_pointer_cast<event>(object_ptr);
            process(hdl, user_oid, event_ptr, response_ptr);

        } else if (object_ptr->tp() == message::stp()) {

            auto message_ptr = std::dynamic_pointer_cast<message>(object_ptr);
            response_ptr->set_command_id(message_ptr->get_id());
            process(hdl, user_oid, message_ptr, response_ptr);

	        send(hdl, response_ptr);

        } else if (object_ptr->tp() == swarm::command::stp()) {

            auto command_ptr = std::dynamic_pointer_cast<swarm::command>(object_ptr);
            response_ptr->set_command_id(command_ptr->get_id());
            auto payload_ptr = command_ptr->get_pl();
            if (payload_ptr->tp() == create_user::stp()) {
                auto ptr = std::dynamic_pointer_cast<create_user>(payload_ptr);
                process(hdl, user_oid, ptr, response_ptr);
            } else if (payload_ptr->tp() == login::stp()) {
                auto ptr = std::dynamic_pointer_cast<login>(payload_ptr);
                process(hdl, user_oid, ptr, response_ptr);
            } else if (payload_ptr->tp() == logout::stp()) {
                auto ptr = std::dynamic_pointer_cast<logout>(payload_ptr);
                process(hdl, user_oid, ptr, response_ptr);
            } else if (payload_ptr->tp() == create_channel::stp()) {
                auto ptr = std::dynamic_pointer_cast<create_channel>(payload_ptr);
                process(hdl, user_oid, ptr, response_ptr);
            } else if (payload_ptr->tp() == look_for_channel::stp()) {
                auto ptr = std::dynamic_pointer_cast<look_for_channel>(payload_ptr);
                process(hdl, user_oid, ptr, response_ptr);
            } else if (payload_ptr->tp() == look_for_user::stp()) {
                auto ptr = std::dynamic_pointer_cast<look_for_user>(payload_ptr);
                process(hdl, user_oid, ptr, response_ptr);
            } else if (payload_ptr->tp() == come_in::stp()) {
                auto ptr = std::dynamic_pointer_cast<come_in>(payload_ptr);
                process(hdl, user_oid, ptr, response_ptr);
            } else if (payload_ptr->tp() == come_out::stp()) {
                auto ptr = std::dynamic_pointer_cast<come_out>(payload_ptr);
                process(hdl, user_oid, ptr, response_ptr);
            } else if (payload_ptr->tp() == get_status::stp()) {
                auto ptr = std::dynamic_pointer_cast<get_status>(payload_ptr);
                process(hdl, user_oid, ptr, response_ptr);
            } else if (payload_ptr->tp() == get_user_list::stp()) {
                auto ptr = std::dynamic_pointer_cast<get_user_list>(payload_ptr);
                process(hdl, user_oid, ptr, response_ptr);
            } else if (payload_ptr->tp() == rename_user::stp()) {
                auto ptr = std::dynamic_pointer_cast<rename_user>(payload_ptr);
                process(hdl, user_oid, ptr, response_ptr);
            } else if (payload_ptr->tp() == get_history::stp()) {
                auto ptr = std::dynamic_pointer_cast<get_history>(payload_ptr);
                process(hdl, user_oid, ptr, response_ptr);
            } else {
                response_ptr->set_code(SMESS001_ERROR);
                response_ptr->set_description(smessage(SMESS001_ERROR));
            }

	        send(hdl, response_ptr);
        }
    } catch (const std::exception& e) {
        lo::l(lo::ERROR) << e.what();
        response_ptr->set_code(SMESS001_ERROR);
        response_ptr->set_description(smessage(SMESS001_ERROR));
	    send(hdl, response_ptr);
    }
}

bool processor::check_name(sptr_cstr nickname, size_t length_min, size_t length_max)
{
    static boost::regex reg(std::string("^[") + ALPH_ALL + "]*$");
	auto nickname_length = util::length_utf8(*nickname);
    if (!(nickname_length >= length_min && nickname_length <= length_max)) return false;
    if (!boost::regex_match(*nickname, reg)) return false;
    return true;
}

void processor::broadcast(swarm::connection hdl, OID channel_oid, OID recipient_oid, std::shared_ptr<swarm::object> object_ptr)
{
    std::list<swarm::connection> connections;

    if (recipient_oid != 0) {
        auto connections_temp = m_connections.get_connection(recipient_oid);
        connections.swap(connections_temp);
    } else if (channel_oid != 0) {
        auto key_ptr = key_channel_user_channel::create();
        key_ptr->set_channel_oid(channel_oid);
        auto answer_ptr = m_bl->find(key_ptr);
        for (auto& record_ptr : *(answer_ptr->get_result())) {
            auto user_oid = channel_user::convert(record_ptr->get_payload())->get_user_oid();
            try {
                auto connections_temp = m_connections.get_connection(user_oid);
                for (auto hdl_local : connections_temp) connections.push_back(hdl_local);
            } catch (...) {
                lo::l(lo::DEBUG) << "(processor::broadcast) Can't find connection for user: " << user_oid;
            }
        }
    }

    for (auto hdl_local : connections) {
        try {
            if (!std::owner_less<swarm::connection>()(hdl, hdl_local) && !std::owner_less<swarm::connection>()(hdl_local, hdl)) continue;

            auto str_ptr = object_ptr->serialization();
            lo::l(lo::DEBUG) << "Send data: " << *str_ptr;
            m_server.send(hdl_local, *str_ptr, websocketpp::frame::opcode::text);

        } catch (const std::exception& e) {
            lo::l(lo::ERROR) << "(processor::broadcast): " << e.what();
        } catch (const websocketpp::lib::error_code& e) {
            lo::l(lo::ERROR) << "(processor::broadcast): " << e.message();
        } catch (...) {
            lo::l(lo::ERROR) << "(processor::broadcast): Unknown";
        }
    }
}

void processor::send(swarm::connection hdl, const std::string& str)
{
    lo::l(lo::DEBUG) << "Send data: " << str;
    try {
        m_server.send(hdl, str, websocketpp::frame::opcode::text);
    } catch (const std::exception& e) {
        lo::l(lo::ERROR) << "(context::send): " << e.what();
    } catch (const websocketpp::lib::error_code& e) {
        lo::l(lo::ERROR) << "(context::send): " << e.message();
    } catch (...) {
        lo::l(lo::ERROR) << "(context::send): Unknown";
    }
}

void processor::send(swarm::connection hdl, std::shared_ptr<swarm::object> obj)
{
    send(hdl, *obj->serialization());
}

swarm::connection_store& processor::get_connection_store()
{
    return m_connections;
}

bool processor::check_authorizetion(OID user_oid, std::shared_ptr<swarm::response>& response_ptr)
{
    if (user_oid == 0) {
        response_ptr->set_code(SMESS201_UNAUTHORIZED);
        response_ptr->set_description(smessage(SMESS201_UNAUTHORIZED));
        return false;
    }
    return true;
}

// +++++++++++++++++++++ COMMANDS ++++++++++++++++++++++++++

void processor::process(swarm::connection hdl, OID user_oid, std::shared_ptr<event>& event_ptr, std::shared_ptr<swarm::response>& response_ptr)
{
    if (check_authorizetion(user_oid, response_ptr)) {
        broadcast(hdl, event_ptr->get_channel_oid(), event_ptr->get_recipient_oid(), event_ptr);
    }
}

void processor::process(swarm::connection hdl, OID user_oid, std::shared_ptr<message>& message_ptr, std::shared_ptr<swarm::response>& response_ptr)
{
    if (check_authorizetion(user_oid, response_ptr)) {
        message_ptr->set_sender_oid(user_oid);
        message_ptr->set_time(time(NULL));
        m_bl->insert(message_ptr);
        broadcast(hdl, message_ptr->get_channel_oid(), message_ptr->get_recipient_oid(), message_ptr);
    }
}

void processor::process(swarm::connection hdl, OID user_oid, std::shared_ptr<create_user>& command_ptr, std::shared_ptr<swarm::response>& response_ptr)
{
    if (!check_name(command_ptr->get_nickname(), cfg().nickname_min, cfg().nickname_max)) {
		response_ptr->set_code(SMESS104_NICKNAME_WRONG);
		response_ptr->set_description(smessage(SMESS104_NICKNAME_WRONG, format(cfg().nickname_min), format(cfg().nickname_max)));
        return;
    }

    auto key_ptr = key_user_nickname::create();
    key_ptr->set_nickname(command_ptr->get_nickname());
    auto user_list = m_bl->find(key_ptr);
    if (user_list->get_result()->size() == 1) {
        response_ptr->set_code(SMESS105_NICKNAME_EXIST);
        response_ptr->set_description(smessage(SMESS105_NICKNAME_EXIST));
        return;
    }

    auto salt_ptr = util::get_random_string(ALPH_ENGLISH ALPH_NUMBERS, 4);
    std::string salt_pass = *salt_ptr + *command_ptr->get_password();

    lo::l(lo::TRASH) << "(processor::process (create_user)) salt: " << *salt_ptr;
    lo::l(lo::TRASH) << "(processor::process (create_user)) salt_pass: " << salt_pass;

    auto user_ptr = user::create();
    user_ptr->set_nickname(command_ptr->get_nickname());
    user_ptr->set_salt(salt_ptr);
    user_ptr->set_hash(std::hash<std::string>()(salt_pass));

    auto oid = m_bl->insert(user_ptr);
    if (oid == 0) {
        response_ptr->set_code(SMESS001_ERROR);
        response_ptr->set_description(smessage(SMESS001_ERROR));
        return;
    }

    lo::l(lo::TRASH) << "(processor::process (create_user)) hash: " << user_ptr->get_hash();

    auto password_hash_ptr = password_hash::create();
    password_hash_ptr->set_value(format(user_ptr->get_hash()));

    // Отправляем hash клиенту
    response_ptr->get_result()->push_back(password_hash_ptr);
}

void processor::process(swarm::connection hdl, OID user_oid, std::shared_ptr<login>& command_ptr, std::shared_ptr<swarm::response>& response_ptr)
{
    if (!check_name(command_ptr->get_nickname(), cfg().nickname_min, cfg().nickname_max)) {
		response_ptr->set_code(SMESS104_NICKNAME_WRONG);
		response_ptr->set_description(smessage(SMESS104_NICKNAME_WRONG, format(cfg().nickname_min), format(cfg().nickname_max)));
        return;
    }

    auto key_ptr = key_user_nickname::create();
    key_ptr->set_nickname(command_ptr->get_nickname());
    auto user_list = m_bl->find(key_ptr);
    if (user_list->get_result()->size() != 1) {
        response_ptr->set_code(SMESS001_ERROR);
        response_ptr->set_description(smessage(SMESS001_ERROR));
        return;
    }

    auto user_ptr = user::convert(user_list->get_result()->front()->get_payload());
    size_tt hash = 0;
    if (command_ptr->get_password()->empty()) hash = format<size_tt>(command_ptr->get_hash());
    else {
        auto salt_pass = *user_ptr->get_salt();
        lo::l(lo::TRASH) << "(processor::process (login)) salt: " << salt_pass;
        salt_pass += *command_ptr->get_password();
        lo::l(lo::TRASH) << "(processor::process (login)) salt_pass: " << salt_pass;
        hash = std::hash<std::string>()(salt_pass);
        lo::l(lo::TRASH) << "(processor::process (login)) hash: " << hash;
    }

    if (user_ptr->get_hash() != hash) {
        response_ptr->set_code(SMESS116_PASSWORD_WRONG);
        response_ptr->set_description(smessage(SMESS116_PASSWORD_WRONG));
        return;
    }

    auto password_hash_ptr = password_hash::create();
    password_hash_ptr->set_value(format(hash));
    response_ptr->get_result()->push_back(password_hash_ptr);
    try {
        m_connections.add(hdl, user_ptr->get_oid().value());
    } catch (...) {
        // повторная авторизация. не будем писать об ошибке
    }
}

void processor::process(swarm::connection hdl, OID user_oid, std::shared_ptr<logout>& command_ptr, std::shared_ptr<swarm::response>& response_ptr)
{
    m_connections.del(hdl);
}

void processor::process(swarm::connection hdl, OID user_oid, std::shared_ptr<create_channel>& command_ptr, std::shared_ptr<swarm::response>& response_ptr)
{
    if (check_name(command_ptr->get_name(), cfg().channelname_min, cfg().channelname_max)) {
        auto channel_ptr = channel::create();
        channel_ptr->set_name(command_ptr->get_name());
        channel_ptr->set_description(command_ptr->get_description());
        auto oid = m_bl->insert(channel_ptr);
        if (oid == 0) {
            response_ptr->set_code(SMESS001_ERROR);
            response_ptr->set_description(smessage(SMESS001_ERROR));
        }
    } else {
		response_ptr->set_code(SMESS106_ROOMNAME_WRONG);
		response_ptr->set_description(smessage(SMESS106_ROOMNAME_WRONG, format(cfg().nickname_min), format(cfg().nickname_max)));
    }
}

void processor::process(swarm::connection hdl, OID user_oid, std::shared_ptr<look_for_channel>& command_ptr, std::shared_ptr<swarm::response>& response_ptr)
{
    auto length = util::length_utf8(*command_ptr->get_text());
    auto answer_ptr = std::make_shared<const swarm::answer>();

    if (length == 0) {
        answer_ptr = m_bl->get_with_limit<channel>(0, 8);
    } else if (length == 3) {
        auto key_ptr = key_channel_name3::create();
        key_ptr->set_name(command_ptr->get_text());
        answer_ptr = m_bl->find(key_ptr);
    } else if (length == 4) {
        auto key_ptr = key_channel_name4::create();
        key_ptr->set_name(command_ptr->get_text());
        answer_ptr = m_bl->find(key_ptr);
    } else if (length == 5) {
        auto key_ptr = key_channel_name5::create();
        key_ptr->set_name(command_ptr->get_text());
        answer_ptr = m_bl->find(key_ptr);
    } else if (length == 6) {
        auto key_ptr = key_channel_name6::create();
        key_ptr->set_name(command_ptr->get_text());
        answer_ptr = m_bl->find(key_ptr);
    } else {
        auto key_ptr = key_channel_name::create();
        key_ptr->set_name(command_ptr->get_text());
        answer_ptr = m_bl->find(key_ptr);
    }

    auto channels = std::make_shared<std::list<std::shared_ptr<const swarm::object>>>();
    for (auto& record_ptr : *(answer_ptr->get_result())) {
        channels->push_back(record_ptr->get_payload());
    }
    response_ptr->set_result(channels);
}

void processor::process(swarm::connection hdl, OID user_oid, std::shared_ptr<look_for_user>& command_ptr, std::shared_ptr<swarm::response>& response_ptr)
{
    auto length = util::length_utf8(*command_ptr->get_text());
    auto answer_ptr = std::make_shared<const swarm::answer>();

    if (length == 0) {
        answer_ptr = m_bl->get_with_limit<user>(0, 8);
    } else if (length == 3) {
        auto key_ptr = key_user_nickname3::create();
        key_ptr->set_nickname(command_ptr->get_text());
        answer_ptr = m_bl->find(key_ptr);
    } else if (length == 4) {
        auto key_ptr = key_user_nickname4::create();
        key_ptr->set_nickname(command_ptr->get_text());
        answer_ptr = m_bl->find(key_ptr);
    } else if (length == 5) {
        auto key_ptr = key_user_nickname5::create();
        key_ptr->set_nickname(command_ptr->get_text());
        answer_ptr = m_bl->find(key_ptr);
    } else if (length == 6) {
        auto key_ptr = key_user_nickname6::create();
        key_ptr->set_nickname(command_ptr->get_text());
        answer_ptr = m_bl->find(key_ptr);
    } else {
        auto key_ptr = key_user_nickname::create();
        key_ptr->set_nickname(command_ptr->get_text());
        answer_ptr = m_bl->find(key_ptr);
    }

    auto users = std::make_shared<std::list<std::shared_ptr<const swarm::object>>>();
    for (auto& record_ptr : *(answer_ptr->get_result())) {
        auto user_ptr = user::convert(record_ptr->get_payload()); 
        auto found_user_ptr = found_user::create();
        found_user_ptr->set_user_oid(user_ptr->get_oid().value());
        found_user_ptr->set_nickname(user_ptr->get_nickname());
        users->push_back(found_user_ptr);
    }
    response_ptr->set_result(users);
}

void processor::process(swarm::connection hdl, OID user_oid, std::shared_ptr<come_in>& command_ptr, std::shared_ptr<swarm::response>& response_ptr)
{
    lo::l(lo::DEBUG) << "(processor::process (come_in)), user_oid: " << user_oid;
    if (!check_authorizetion(user_oid, response_ptr)) return;
    try {
        lo::l(lo::DEBUG) << "Get channel with channel_oid = " << command_ptr->get_channel_oid();
        auto channel_ptr = m_bl->get<channel>(command_ptr->get_channel_oid());
    } catch (...) {
        lo::l(lo::ERROR) << "The channel with channel_oid = " << command_ptr->get_channel_oid() << " wasn't found";
        response_ptr->set_code(SMESS001_ERROR);
        response_ptr->set_description(smessage(SMESS001_ERROR));
        return;
    }
    auto channel_user_ptr = channel_user::create();
    channel_user_ptr->set_channel_oid(command_ptr->get_channel_oid());
    channel_user_ptr->set_user_oid(user_oid);
    auto oid = m_bl->insert(channel_user_ptr);
    if (oid != 0) {
        // Отправляем уведомление всем пользователям о том, что подключился пользователь
        try {
            auto user_ptr = user::convert(m_bl->get_object<user>(user_oid));
            auto inside_ptr = inside::create();
            inside_ptr->set_user_oid(user_oid);
            inside_ptr->set_nickname(user_ptr->get_nickname());
            inside_ptr->set_flag(true);
            auto event_ptr = event::create();
            event_ptr->set_channel_oid(command_ptr->get_channel_oid());
            event_ptr->set_payload(inside_ptr);
            broadcast(hdl, command_ptr->get_channel_oid(), 0, event_ptr);
        } catch (...) {}
    }
}

void processor::process(swarm::connection hdl, OID user_oid, std::shared_ptr<come_out>& command_ptr, std::shared_ptr<swarm::response>& response_ptr)
{
    lo::l(lo::DEBUG) << "(processor::process (come_out)), user_oid: " << user_oid;
    if (!check_authorizetion(user_oid, response_ptr)) return;
    auto key_ptr = key_channel_user_channel_user::create();
    key_ptr->set_channel_oid(command_ptr->get_channel_oid());
    key_ptr->set_user_oid(user_oid);
    auto answer_ptr = m_bl->find(key_ptr);
    if (answer_ptr->get_result()->size() == 1) {
        // Отправляем уведомление всем пользователям о том, что пользователь отключился
        try {
            auto user_ptr = user::convert(m_bl->get_object<user>(user_oid));
            auto inside_ptr = inside::create();
            inside_ptr->set_user_oid(user_oid);
            inside_ptr->set_nickname(user_ptr->get_nickname());
            inside_ptr->set_flag(false);
            auto event_ptr = event::create();
            event_ptr->set_channel_oid(command_ptr->get_channel_oid());
            event_ptr->set_payload(inside_ptr);
            broadcast(hdl, command_ptr->get_channel_oid(), 0, event_ptr);
        } catch (...) {}
    } else {
        response_ptr->set_code(SMESS001_ERROR);
        response_ptr->set_description(smessage(SMESS001_ERROR));
        return;
    }
    m_bl->remove<channel_user>(answer_ptr->get_result()->front()->get_index());
}

void processor::process(swarm::connection hdl, OID user_oid, std::shared_ptr<get_status>& command_ptr, std::shared_ptr<swarm::response>& response_ptr)
{
    auto status_ptr = status::create();
    status_ptr->set_user_oid(user_oid);
    if (user_oid != 0) {
        auto user_ptr = user::convert(m_bl->get_object<user>(user_oid));
        status_ptr->set_nickname(user_ptr->get_nickname());
        status_ptr->set_created(user_ptr->get_created());

        auto key_ptr = key_channel_user_user::create();
        key_ptr->set_user_oid(user_oid);
        auto answer_ptr = m_bl->find(key_ptr);

        // Заполняем каналами, к которым подключен пользователь
        for (auto& record_ptr : *(answer_ptr->get_result())) {
            auto channel_oid = channel_user::convert(record_ptr->get_payload())->get_channel_oid();
            auto channel_ptr = channel::convert(m_bl->get_object<channel>(channel_oid));
            status_ptr->get_channels()->push_back(channel_ptr);
        }

        // Заполняем подключениями клиентов
        try {
            auto connections_temp = m_connections.get_connection(user_oid);
            status_ptr->set_connections_amount(connections_temp.size());
        } catch (...) {
            lo::l(lo::DEBUG) << "(processor::process) Can't find connection for user: " << user_oid;
        }
    }
    response_ptr->get_result()->push_back(status_ptr);
}

void processor::process(swarm::connection hdl, OID user_oid, std::shared_ptr<get_user_list>& command_ptr, std::shared_ptr<swarm::response>& response_ptr)
{
    auto key_ptr = key_channel_user_channel::create();
    key_ptr->set_channel_oid(command_ptr->get_channel_oid());
    auto answer_ptr = m_bl->find(key_ptr);

    auto users = std::make_shared<std::list<std::shared_ptr<const swarm::object>>>();
    for (auto& record_ptr : *(answer_ptr->get_result())) {
        auto channel_user_ptr = channel_user::convert(record_ptr->get_payload());
        auto user_ptr = user::convert(m_bl->get_object<user>(channel_user_ptr->get_user_oid()));
        auto found_user_ptr = found_user::create();
        found_user_ptr->set_user_oid(user_ptr->get_oid().value());
        found_user_ptr->set_nickname(user_ptr->get_nickname());
        users->push_back(found_user_ptr);
    }
    response_ptr->set_result(users);
}

void processor::process(swarm::connection hdl, OID user_oid, std::shared_ptr<rename_user>& command_ptr, std::shared_ptr<swarm::response>& response_ptr)
{
    if (!check_authorizetion(user_oid, response_ptr)) return;

    if (!check_name(command_ptr->get_nickname(), cfg().nickname_min, cfg().nickname_max)) {
		response_ptr->set_code(SMESS104_NICKNAME_WRONG);
		response_ptr->set_description(smessage(SMESS104_NICKNAME_WRONG, format(cfg().nickname_min), format(cfg().nickname_max)));
        return;
    }

    auto user_ptr = user::convert(m_bl->get_object<user>(user_oid));
    auto user_new_ptr = std::dynamic_pointer_cast<user>(user_ptr->make_copy());
    user_new_ptr->set_nickname(command_ptr->get_nickname());
    // Можно использовать user_oid, т.к. в данном случае он совпадает с индексом
    auto oid = m_bl->update(user_oid, user_new_ptr);
    if (oid == 0) {
        response_ptr->set_code(SMESS105_NICKNAME_EXIST);
        response_ptr->set_description(smessage(SMESS105_NICKNAME_EXIST));
        return;
    }

    auto changed_nickname_ptr = changed_nickname::create();
    changed_nickname_ptr->set_user_oid(user_new_ptr->get_oid().value());
    changed_nickname_ptr->set_nickname(user_new_ptr->get_nickname());
    changed_nickname_ptr->set_nickname_past(user_ptr->get_nickname());
    auto event_ptr = event::create();
    event_ptr->set_payload(changed_nickname_ptr);

    // Отправляем уведомления во все каналы, к которым подключен пользователь
    auto key_ptr = key_channel_user_user::create();
    key_ptr->set_user_oid(user_oid);
    auto answer_ptr = m_bl->find(key_ptr);
    for (auto& record_ptr : *(answer_ptr->get_result())) {
        auto channel_oid = channel_user::convert(record_ptr->get_payload())->get_channel_oid();
        broadcast(hdl, channel_oid, 0, event_ptr);
    }
}

void processor::process(swarm::connection hdl, OID user_oid, std::shared_ptr<get_history>& command_ptr, std::shared_ptr<swarm::response>& response_ptr)
{
    if (!check_authorizetion(user_oid, response_ptr)) return;

    auto answer_ptr = std::make_shared<const swarm::answer>();
    auto channel_oid = command_ptr->get_channel_oid();

    if (channel_oid == 0) {
        auto key_lower_ptr = key_message_recipient::create();
        key_lower_ptr->set_recipient_oid(user_oid);
        key_lower_ptr->set_time(command_ptr->get_time());

        auto key_upper_ptr = key_message_recipient::create();
        key_upper_ptr->set_recipient_oid(user_oid - 1);
        key_upper_ptr->set_time(static_cast<time_tt>(0) - 1);

        answer_ptr = m_bl->find_range(key_lower_ptr, key_upper_ptr, 0, command_ptr->get_limit(),
            [channel_oid](const swarm::object& obj){FROM(message) WHERE(FL(channel_oid) == 0)}
        );
    } else {
        auto key_lower_ptr = chatd::key_message_descend::create();
        key_lower_ptr->set_channel_oid(channel_oid);
        key_lower_ptr->set_time(command_ptr->get_time());

        auto key_upper_ptr = chatd::key_message_descend::create();
        key_upper_ptr->set_channel_oid(channel_oid - 1);
        key_upper_ptr->set_time(static_cast<time_tt>(0) - 1);

        answer_ptr = m_bl->find_range(key_lower_ptr, key_upper_ptr, 0, command_ptr->get_limit(),
            [channel_oid, user_oid](const swarm::object& obj){FROM(message) WHERE(
                FL(channel_oid) == channel_oid && (FL(recipient_oid) == 0 || FL(recipient_oid) == user_oid)
            )}
        );
    }
    for (auto& record_ptr : *(answer_ptr->get_result())) {
        response_ptr->get_result()->push_back(record_ptr->get_payload());
    }
}

} // end of namespace
