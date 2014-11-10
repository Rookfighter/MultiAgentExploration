#include "algorithm-rt/NodeCounting.hpp"
#include "algorithm-rt/InitialAntState.hpp"

namespace mae
{
	NodeCounting::NodeCounting(const AlgorithmConfig &p_config)
		:AntAlgorithm()
	{
		AntStateProperties properties;
		properties.robot = p_config.robot;
		properties.stock = p_config.stock;
		properties.updateValue = std::bind(&NodeCounting::updateValue, this, std::placeholders::_1, std::placeholders::_2);
		properties.calcValue = std::bind(&NodeCounting::calcValue, this, std::placeholders::_1, std::placeholders::_2);
		properties.obstacleAvoidDistance = p_config.obstacleAvoidDistance;
		properties.obstacleMarkerDistance = p_config.obstacleMarkerDistance;
		
		init(new InitialAntState(properties));
	}
	
	NodeCounting::~NodeCounting()
	{
	}
	
	void NodeCounting::updateValue(Marker *p_current, Marker *p_next)
	{
		assert(p_current != NULL);
		p_current->changeValueBy(1.0);
	}
	
	double NodeCounting::calcValue(Marker *p_current, Marker *p_next)
	{
		assert(p_next != NULL);
		return p_next->getValue();
	}
}
