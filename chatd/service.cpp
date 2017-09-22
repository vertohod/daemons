#include <exception>

#include "service.h"
#include "object.h"
#include "ping.h"
#include "pong.h"
#include "log.h"

using websocketpp::lib::placeholders::_1;                                                                                                                                                                                                     
using websocketpp::lib::placeholders::_2;                                                                                                                                                                                                     
using websocketpp::lib::bind;

namespace chatd
{

service::service() : m_processor(m_server)
{
    m_server.init_asio();
    m_server.set_open_handler(bind(&service::on_open, this, ::_1));
    m_server.set_close_handler(bind(&service::on_close, this, ::_1));
    m_server.set_message_handler(bind(&service::on_message, this, ::_1, ::_2));
    m_server.clear_access_channels(websocketpp::log::alevel::all);
    m_server.clear_error_channels(websocketpp::log::alevel::all);

    m_processor.start(1); // запустили один поток обработки
}

void service::run(unsigned int port)
{
    m_server.listen(port);
    m_server.start_accept();

    try {
        m_server.run();
    } catch (const std::exception& e) {
        lo::l(lo::ERROR) << "service::run(): " << e.what();
    }
}

void service::on_open(swarm::connection hdl)
{
}

void service::on_close(swarm::connection hdl)
{
    try {
        m_processor.get_connection_store().del(hdl);
    } catch (const std::exception& e) {
        lo::l(lo::ERROR) << "service::on_close(): " << e.what();
    }
}

void service::on_message(swarm::connection hdl, server_t::message_ptr msg)
{
    static auto pong_ptr = pong::create();

    try {
        lo::l(lo::DEBUG) << "Were received data (" << msg->get_payload().size() << "): " << msg->get_payload();

        auto object_name = swarm::object::get_field(msg->get_payload(), FIELD_OBJECT_TYPE);

        if (object_name->empty()) {
            lo::l(lo::ERROR) << "Were received unknown JSON: " << msg->get_payload();
            return;
        }

        auto object_ptr = swarm::object::create_object(object_name);
        object_ptr->deserialization(msg->get_payload());

        if (object_ptr->tp() == ping::stp()) {
            pong_ptr->set_id(ping::convert(object_ptr)->get_id());
            pong_ptr->set_time(time(NULL));
            m_processor.send(hdl, *pong_ptr->get_object()->serialization());
        } else m_processor.add_command(hdl, object_ptr);

    } catch (const std::exception& e) {
        lo::l(lo::ERROR) << "service::on_message(): " << e.what();
    }
}

} // end of namespace
