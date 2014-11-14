#include <easylogging++.h>
#include "test/NodeCountingTest.hpp"

namespace mae
{

	NodeCountingTest::NodeCountingTest(Simulation *p_world,
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
			logicWatch_.start();
			nodeCounting_->update();
			logicWatch_.stop();
			if(logicWatch_.hasNewWorstCase())
				LOG(INFO) << "LogicLag: " << logicWatch_.getWorstMsec() << "ms";
		}
	}


}
