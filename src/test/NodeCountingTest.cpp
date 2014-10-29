#include <easylogging++.h>
#include "test/NodeCountingTest.hpp"

namespace mae
{

	NodeCountingTest::NodeCountingTest(World *p_world,
	                                   const std::string &p_robotName)
		:world_(p_world),
		 nodeCounting_(p_world->getRobot(p_robotName), p_world->getSimulation(), p_world->getStock())
	{
	}

	NodeCountingTest::~NodeCountingTest()
	{
	}

	void NodeCountingTest::execute()
	{
		while(true) {
			world_->update();
			watch_.start();
			nodeCounting_.step();
			watch_.stop();
			//LOG(INFO) << "Step: " << watch_.str();
		}
	}


}
