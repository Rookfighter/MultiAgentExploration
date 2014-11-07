#include <easylogging++.h>
#include "app/Application.hpp"
#include "algorithm/ExperimentLoader.hpp"

namespace mae
{

	Application::Application(int argc, char** argv)
	{
		assert(argc > 1);
		
		experiment_ = ExperimentLoader::load(argv[1]);
	}

	Application::~Application()
	{
	}
	
	void Application::run()
	{
		LOG(INFO) << "Running Application";
		try {
			while(true)
				experiment_->step();
		} catch(std::exception &e) {
			LOG(WARNING) << "Catched exception: " << e.what();
		} catch(...) {
			LOG(WARNING) << "Catched unkown instance.";
		}
	}

}
