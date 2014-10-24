#include <easylogging++.h>
#include "test/TestApplication.hpp"
#include "simulation/WorldLoader.hpp"

namespace mae
{

	TestApplication::TestApplication()
	{
		world_ = WorldLoader::load("test_simulation.yaml");
		basicTest_ = new BasicTest(*world_->getClient(),
		                           *world_->getSimulation(),
		                           *world_->getStock(),
		                           *world_->getRobot("pioneer1"));
		heisenbergTest_ = new HeisenbergTest(*world_->getClient(),
		                                     *world_->getRobot("pioneer1"));
		wanderTest_ = new WanderTest(*world_->getClient(),
		                             *world_->getRobot("pioneer1"));
		ncTest_ = new NodeCountingTest(world_->getClient(),
		                               world_->getRobot("pioneer1"),
		                               world_->getSimulation(),
		                               world_->getStock());
	}

	TestApplication::~TestApplication()
	{
		delete ncTest_;
		delete heisenbergTest_;
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
		} catch(PlayerCc::PlayerError &e) {
			LOG(WARNING) << "Catched player error: " << e.GetErrorStr();
		} catch(...) {
			LOG(WARNING) << "Catched unkown instance.";
		}
	}


}
