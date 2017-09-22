#ifndef CONFIGURE_H
#define CONFIGURE_H

#include <boost/thread/detail/singleton.hpp>
#include <string>
#include "types.h"

#include "config.h"

class chatd_config : public config
{
public:
    chatd_config() {}

    chatd_config(const std::string& arPath) : config(arPath)
    {
        call_back();
    }

    void call_back()
    {
        // Необходим вызов для установки базовых параметров
        config::call_back();

        auto& op = get_options();

        op
            ("server-id", "Unique ID base server", server_id)
            ("main", "Flag says that the current server is main", main)
            ("listen", "Address for listening server", listen)
            ("listen_base", "Address for listening base", listen_base)
            ("path-pid", "Path of file which content daemon's pid", path_pid)
            ("nickname-min", "Min length for nickname", nickname_min)
            ("nickname-max", "Max length for nickname", nickname_max)
            ("channelname-min", "Min length for channel's name", channelname_min)
            ("channelname-max", "Max length for channel's name", channelname_max)
            ("secret-key", "Secret key for authentication servers with each other", secret_key)
        ;

        // Проверяем, нет ли лишних параметров в файле
        check();
    }


public:
    // OPTIONS --------------------------------------------------------

    OID                 server_id;
    bool                main;
    unsigned int        listen;
    std::string         listen_base;
    std::string         path_pid;
    size_t              nickname_min;
    size_t              nickname_max;
    size_t              channelname_min;
    size_t              channelname_max;
    std::string         secret_key;

    // ----------------------------------------------------------------
};

inline chatd_config& cfg()
{
    return boost::detail::thread::singleton<chatd_config>::instance();
}

#endif
