#include "algorithm/NodeCounting.hpp"
#include "algorithm/InitialAntState.hpp"

namespace mae
{
	static void updateValueNodeCounting(Marker *p_current, Marker *p_next)
	{
		assert(p_current != NULL);
		p_current->incrementValue();
	}

	NodeCounting::NodeCounting(const AlgorithmConfig &p_config)
		:AntAlgorithm()
	{
		AntStateProperties properties;
		properties.robot = p_config.robot;
		properties.stock = p_config.stock;
		properties.simulation = p_config.simulation;
		properties.updateValue = updateValueNodeCounting;
		properties.obstacleAvoidDistance = p_config.obstacleAvoidDistance;
		properties.obstacleMarkerDistance = p_config.obstacleMarkerDistance;
		
		init(new InitialAntState(properties));
	}

	NodeCounting::~NodeCounting()
	{
	}
}
