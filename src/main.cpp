#include <easylogging++.h>
#include "test/TestApplication.hpp"
#include "app/Application.hpp"

_INITIALIZE_EASYLOGGINGPP

int main(int argc, char **argv)
{
	el::Configurations conf("easylog.conf");
	el::Loggers::reconfigureLogger("default", conf);
	
	mae::Application app(argc, argv);
	//mae::TestApplication testApp;
	LOG(INFO) << "App started.";
	app.run();
	LOG(INFO) << "App terminated.";
	return 0;
}
