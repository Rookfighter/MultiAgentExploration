#ifndef MAE_NODE_COUNTING_TEST_HPP
#define MAE_NODE_COUNTING_TEST_HPP

#include "simulation/Simulation.hpp"
#include "algorithm-rt/NodeCounting.hpp"
#include "utils/StopWatch.hpp"

namespace mae
{
	class NodeCountingTest
	{
	private:
		Simulation *world_;
		NodeCounting *nodeCounting_;
		StopWatch updateWatch_;
		StopWatch logicWatch_;
		
	public:
		NodeCountingTest(Simulation *p_world,
		                 const std::string &p_robotName);
		~NodeCountingTest();

		void execute();

	};

}

#endif
