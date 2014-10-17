#include "algorithm/NodeCounting.hpp"

namespace mae
{

	NodeCounting::NodeCounting(ExplorationBot &p_robot,
	                           Simulation &p_simulation,
	                           MarkerStock &p_stock)
		:robot_(p_robot), simulation_(p_simulation), stock_(p_stock)
	{
	}

	NodeCounting::~NodeCounting()
	{
	}

	void NodeCounting::step()
	{

	}
}
