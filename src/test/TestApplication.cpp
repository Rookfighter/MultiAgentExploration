#include "test/TestApplication.hpp"

namespace mae
{

	TestApplication::TestApplication()
	{
		loader_.load("test_simulation.yaml");
		basicTest_ = new BasicTest(*loader_.getClient(),
		                           *loader_.getSimulation(),
		                           *loader_.getStock(),
		                           *loader_.getRobot("pioneer1"));
		heisenbergTest_ = new HeisenbergTest(*loader_.getClient(),
		                                     *loader_.getRobot("pioneer1"));
		wanderTest_ = new WanderTest(*loader_.getClient(),
		                             *loader_.getRobot("pioneer1"));
	}

	TestApplication::~TestApplication()
	{
		delete wanderTest_;
		delete heisenbergTest_;
		delete wanderTest_;
	}

	void TestApplication::run()
	{
		LOG(INFO) << "Running Test";
		try {
			basicTest_->step();
		} catch(std::exception &e) {
			LOG(WARNING) << "Catched exception: " << e.what();
		} catch(PlayerCc::PlayerError &e) {
			LOG(WARNING) << "Catched player error: " << e.GetErrorStr();
		}
	}


}
