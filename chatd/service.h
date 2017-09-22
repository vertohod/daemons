#ifndef SERVICE_H
#define SERVICE_H

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include "connection.h"
#include "processor.h"

namespace chatd
{

class service
{
private:
    typedef websocketpp::server<websocketpp::config::asio> server_t;

    server_t            m_server;
    processor           m_processor;

    void on_open(swarm::connection hdl);
    void on_close(swarm::connection hdl);
    void on_message(swarm::connection hdl, server_t::message_ptr msg);

public:
    service();

    void run(unsigned int port);
};

} // end of namespace

#endif
