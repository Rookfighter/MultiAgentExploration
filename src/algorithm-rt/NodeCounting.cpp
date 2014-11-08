#include "algorithm-rt/NodeCounting.hpp"
#include "algorithm-rt/InitialAntState.hpp"

namespace mae
{
	static void updateValueNodeCounting(Marker *p_current, Marker *p_next)
	{
		assert(p_current != NULL);
		p_current->changeValueBy(1.0);
	}
	
	static double calcValueNodeCounting(Marker *p_current, Marker *p_next)
	{
		assert(p_next != NULL);
		return p_next->getValue();
	}

	NodeCounting::NodeCounting(const AlgorithmConfig &p_config)
		:AntAlgorithm()
	{
		AntStateProperties properties;
		properties.robot = p_config.robot;
		properties.stock = p_config.stock;
		properties.updateValue = updateValueNodeCounting;
		properties.calcValue = calcValueNodeCounting;
		properties.obstacleAvoidDistance = p_config.obstacleAvoidDistance;
		properties.obstacleMarkerDistance = p_config.obstacleMarkerDistance;
		
		init(new InitialAntState(properties));
	}

	NodeCounting::~NodeCounting()
	{
	}
}
