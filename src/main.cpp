#include <easylogging++.h>
#include "test/TestApplication.hpp"
#include "app/Application.hpp"

_INITIALIZE_EASYLOGGINGPP

int main(int argc, char **argv)
{
	el::Configurations conf("config/easylog.conf");
	el::Loggers::reconfigureLogger("default", conf);
	
	mae::Application app(argc, argv);
	app.run();
    LOG(INFO) << "Exiting main";
	return 0;
}
