#include <easylogging++.h>
#include <signal.h>
#include "app/Application.hpp"

_INITIALIZE_EASYLOGGINGPP

static mae::Application *app;

static void sig_handler(int signo)
{
    app->stop();
}

int main(int argc, char **argv)
{
    el::Configurations conf("config/easylog.conf");
    el::Loggers::reconfigureLogger("default", conf);

    app = new mae::Application(argc, argv);
    signal(SIGTERM, sig_handler);
    signal(SIGINT, sig_handler);
    signal(SIGKILL, sig_handler);
    int exitCode = app->run();
    delete app;
    LOG(INFO) << "Exiting main";
    return exitCode;
}
