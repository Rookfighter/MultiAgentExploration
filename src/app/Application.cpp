#include <easylogging++.h>
#include "app/Application.hpp"
#include "algorithm/ExperimentLoader.hpp"

namespace mae
{
	static int experimentCallback(Stg::World* world, void* userarg)
	{
		Experiment *experiment = (Experiment*) userarg;
		experiment->update();
		
		return 0;
	}
	
	Application::Application(int argc, char** argv)
	{
		assert(argc > 1);
		
		Stg::Init(&argc, &argv);
		experiment_ = ExperimentLoader::load(argv[1]);
		experiment_->getSimulation()->getWorld()->AddUpdateCallback(experimentCallback, experiment_);
	}

	Application::~Application()
	{
	}
	
	void Application::run()
	{
		LOG(INFO) << "Running Application";
		try {
			experiment_->getSimulation()->getWorld()->Run();
		} catch(std::exception &e) {
			LOG(WARNING) << "Catched exception: " << e.what();
		} catch(...) {
			LOG(WARNING) << "Catched unkown instance.";
		}
	}

}
