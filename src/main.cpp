#include <easylogging++.h>
#include "test/TestApplication.hpp"

_INITIALIZE_EASYLOGGINGPP

int main(int argc, char **argv)
{
	el::Configurations conf("easylog.conf");
	el::Loggers::reconfigureLogger("default", conf);

	LOG(INFO) << "App started.";
	mae::TestApplication app;
	app.run();
	LOG(INFO) << "App terminated.";
	return 0;
}
