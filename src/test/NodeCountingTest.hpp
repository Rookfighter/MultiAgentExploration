#ifndef MAE_NODE_COUNTING_TEST_HPP
#define MAE_NODE_COUNTING_TEST_HPP

#include "simulation/World.hpp"
#include "algorithm/NodeCounting.hpp"
#include "common/StopWatch.hpp"

namespace mae
{
	class NodeCountingTest
	{
	private:
		World *world_;
		NodeCounting nodeCounting_;
		StopWatch watch_;
	public:
		NodeCountingTest(World *p_world,
		                 const std::string &p_robotName);
		~NodeCountingTest();

		void execute();

	};

}

#endif
