#include "test/NodeCountingTest.hpp"

namespace mae
{

	NodeCountingTest::NodeCountingTest(PlayerClient *p_client,
	                                   ExplorationBot *p_robot,
	                                   Simulation *p_simulation,
	                                   MarkerStock *p_stock)
		:client_(p_client), nodeCounting_(p_robot,p_simulation,p_stock)
	{
	}

	NodeCountingTest::~NodeCountingTest()
	{
	}

	void NodeCountingTest::execute()
	{
		while(true) {
			client_->update();
			nodeCounting_.step();
		}
	}


}
