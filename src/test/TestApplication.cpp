#include <easylogging++.h>
#include "test/TestApplication.hpp"
#include "simulation/WorldLoader.hpp"

namespace mae
{

	TestApplication::TestApplication()
	{
		world_ = WorldLoader::load("test_simulation.yaml");
		wanderTest_ = new WanderTest(*world_,
		                             *world_->getRobot("pioneer1"));
		ncTest_ = new NodeCountingTest(world_, "pioneer1");
		performanceTest_ = new PerformanceTest();
	}

	TestApplication::~TestApplication()
	{
		delete performanceTest_;
		delete ncTest_;
		delete wanderTest_;
		delete world_;
	}

	void TestApplication::run()
	{
		LOG(INFO) << "Running Test";
		try {
			ncTest_->execute();
		} catch(std::exception &e) {
			LOG(WARNING) << "Catched exception: " << e.what();
		} catch(...) {
			LOG(WARNING) << "Catched unkown instance.";
		}
	}


}
