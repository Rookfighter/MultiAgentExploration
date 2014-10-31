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
			updateWatch_.start();
			world_->update();
			updateWatch_.stop();
			if(updateWatch_.hasNewWorstCase())
				LOG(INFO) << "UpdateLag: " << updateWatch_.getWorstMsec() << "ms";
				
			logicWatch_.start();
			nodeCounting_.step();
			logicWatch_.stop();
			if(logicWatch_.hasNewWorstCase())
				LOG(INFO) << "LogicLag: " << logicWatch_.getWorstMsec() << "ms";
		}
	}


}
