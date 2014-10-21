#include "algorithm/NodeCounting.hpp"
#include "algorithm/InitialAntState.hpp"

namespace mae
{
	static void updateValueNodeCounting(Marker *p_current, Marker *p_next)
	{
		assert(p_current != NULL);
		p_current->value++;
	}
	
	NodeCounting::NodeCounting(ExplorationBot *p_robot,
	                           Simulation *p_simulation,
	                           MarkerStock *p_stock)
		:AntAlgorithm()
	{
		AntStateProperties properties;
		properties.robot = p_robot;
		properties.stock = p_stock;
		properties.simulation = p_simulation;
		properties.updateValue = updateValueNodeCounting;
		
		init(new InitialAntState(properties));
	}

	NodeCounting::~NodeCounting()
	{
	}
}
