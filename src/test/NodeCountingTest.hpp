#ifndef MAE_NODE_COUNTING_TEST_HPP
#define MAE_NODE_COUNTING_TEST_HPP

#include "algorithm/NodeCounting.hpp"

namespace mae
{
	class NodeCountingTest
	{
	private:
		PlayerClient *client_;
		NodeCounting nodeCounting_;
	public:
		NodeCountingTest(PlayerClient *p_client,
		                 ExplorationBot *p_robot,
		                 Simulation *p_simulation,
		                 MarkerStock *p_stock);
		~NodeCountingTest();

		void execute();

	};

}

#endif
