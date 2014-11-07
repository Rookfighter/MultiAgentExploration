#include <easylogging++.h>
#include "test/NodeCountingTest.hpp"

namespace mae
{

	NodeCountingTest::NodeCountingTest(World *p_world,
	                                   const std::string &p_robotName)
		:world_(p_world)
	{
		AlgorithmConfig config;
		config.robot = world_->getRobot(p_robotName);
		config.stock = p_world->getStock();
		config.obstacleAvoidDistance = 0.5;
		config.obstacleMarkerDistance = 1.0;
		nodeCounting_ = new NodeCounting(config);
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
			nodeCounting_->step();
			logicWatch_.stop();
			if(logicWatch_.hasNewWorstCase())
				LOG(INFO) << "LogicLag: " << logicWatch_.getWorstMsec() << "ms";
		}
	}


}
