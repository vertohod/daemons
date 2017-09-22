#include <exception>
#include <execinfo.h>
#include <signal.h>

#include "stat_daemon.h"
#include "configure.h"
#include "service.h"
#include "util.h"
#include "log.h"

void print_backtrace(int sig)
{
    constexpr int ARRAYSIZE = 50;

    void *array[ARRAYSIZE];
    char **strings;

    int size = backtrace(array, ARRAYSIZE);
    strings = backtrace_symbols(array, size);

    lo::l(lo::ERROR) << "Signal: " << sig;
    if (strings != nullptr) {
        for (int count = 0; count < size; ++count) {
            lo::l(lo::ERROR) << strings[count];
        }
    }
    free(strings);
}

int main(int argc, char* argv[])
{
    signal(SIGSEGV, print_backtrace);
    signal(SIGABRT, print_backtrace);

    try {
        lo::set_log_level(lo::TRASH);

        cfg().parse_cmd(argc, argv);

        // Перенаплавляем STDOUT в файл
        lo::l(lo::WARNING) << "Forwarding STDOUT to " << cfg().path_to_stdout;
        lo::set_output(cfg().path_to_stdout);
        // Демонизируемся
        util::daemonize();
        util::savepid(cfg().path_pid);

        // Создаем объект профилировщика
//        auto& prof = singleton<profile<stat_daemon>>::instance();
//        prof.clear();

        lo::l(lo::WARNING) << "Daemon chatd is started, listen: " << cfg().listen;
//        lo::set_log_level(lo::WARNING);

        chatd::service srv;
        srv.run(cfg().listen);

    } catch (const std::exception& e) {
        lo::l(lo::FATAL) << "main: " << e.what();
    } catch (...) {
        lo::l(lo::FATAL) << "main: Unknown exception";
    }

    return 0;
}
