#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <websocketpp/config/asio_no_tls.hpp>                                                                                                                                                                                                 
#include <websocketpp/server.hpp>
#include <condition_variable>
#include <functional>
#include <memory>
#include <vector>
#include <thread>
#include <mutex>
#include <deque>

#include "connection_store.h"
#include "connection.h"
#include "swarm.h"
#include "object.h"

// list of commands headers
#include "swarm/commands/response.h"
#include "look_for_channel.h"
#include "create_channel.h"
#include "look_for_user.h"
#include "get_user_list.h"
#include "get_history.h"
#include "create_user.h"
#include "rename_user.h"
#include "get_status.h"
#include "come_out.h"
#include "come_in.h"
#include "message.h"
#include "logout.h"
#include "login.h"
#include "event.h"

namespace chatd
{

class processor
{
private:
    typedef websocketpp::server<websocketpp::config::asio> server_t;
	typedef std::deque<std::pair<swarm::connection, std::shared_ptr<swarm::object>>> objects_t;
    objects_t m_objects;
    std::vector<std::shared_ptr<std::thread>> m_threads;
    server_t& m_server;

    std::mutex m_mutex;
    bool m_stop_flag;
    bool m_data_flag;
    std::condition_variable m_cv;
    std::shared_ptr<swarm::swarm> m_bl;

    swarm::connection_store m_connections;

private:
    typedef std::unordered_map<OID, swarm::connection> users_t;
    typedef std::unordered_map<OID, users_t> channels_t;
    channels_t m_channels;

public:
    processor(server_t& server);
    ~processor();

    void start(int count_threads);
    void add_command(swarm::connection hdl, std::shared_ptr<swarm::object>& object_ptr);
    swarm::connection_store& get_connection_store();
    void send(swarm::connection hdl, const std::string& str);

private:
    void thread_function();
    void process_commands(swarm::connection hdl, std::shared_ptr<swarm::object>& object_ptr);

private:
	static bool check_name(sptr_cstr nickname, size_t length_min, size_t length_max);

    void process(swarm::connection hdl, OID user_oid, std::shared_ptr<event>& event_ptr, std::shared_ptr<swarm::response>& response_ptr);
    void process(swarm::connection hdl, OID user_oid, std::shared_ptr<message>& message_ptr, std::shared_ptr<swarm::response>& response_ptr);
    void process(swarm::connection hdl, OID user_oid, std::shared_ptr<create_user>& command_ptr, std::shared_ptr<swarm::response>& response_ptr);
    void process(swarm::connection hdl, OID user_oid, std::shared_ptr<login>& command_ptr, std::shared_ptr<swarm::response>& response_ptr);
    void process(swarm::connection hdl, OID user_oid, std::shared_ptr<logout>& command_ptr, std::shared_ptr<swarm::response>& response_ptr);
    void process(swarm::connection hdl, OID user_oid, std::shared_ptr<create_channel>& command_ptr, std::shared_ptr<swarm::response>& response_ptr);
    void process(swarm::connection hdl, OID user_oid, std::shared_ptr<look_for_channel>& command_ptr, std::shared_ptr<swarm::response>& response_ptr);
    void process(swarm::connection hdl, OID user_oid, std::shared_ptr<look_for_user>& command_ptr, std::shared_ptr<swarm::response>& response_ptr);
    void process(swarm::connection hdl, OID user_oid, std::shared_ptr<come_in>& command_ptr, std::shared_ptr<swarm::response>& response_ptr);
    void process(swarm::connection hdl, OID user_oid, std::shared_ptr<come_out>& command_ptr, std::shared_ptr<swarm::response>& response_ptr);
    void process(swarm::connection hdl, OID user_oid, std::shared_ptr<get_status>& command_ptr, std::shared_ptr<swarm::response>& response_ptr);
    void process(swarm::connection hdl, OID user_oid, std::shared_ptr<get_user_list>& command_ptr, std::shared_ptr<swarm::response>& response_ptr);
    void process(swarm::connection hdl, OID user_oid, std::shared_ptr<rename_user>& command_ptr, std::shared_ptr<swarm::response>& response_ptr);
    void process(swarm::connection hdl, OID user_oid, std::shared_ptr<get_history>& command_ptr, std::shared_ptr<swarm::response>& response_ptr);

private:
    bool check_authorizetion(OID user_oid, std::shared_ptr<swarm::response>& response_ptr);
    void broadcast(swarm::connection hdl, OID channel_oid, OID recipient_oid, std::shared_ptr<swarm::object> object_ptr);
    void send(swarm::connection hdl, std::shared_ptr<swarm::object> obj);
};

} // end of namespace
#endif
