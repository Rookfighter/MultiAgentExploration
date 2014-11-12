#include "algorithm-rt/NodeCounting.hpp"

namespace mae
{
	NodeCounting::NodeCounting(const AlgorithmConfig &p_config)
		:AntAlgorithm(p_config,
                      std::bind(&NodeCounting::updateValue, this, std::placeholders::_1, std::placeholders::_2),
                      std::bind(&NodeCounting::calcValue, this, std::placeholders::_1, std::placeholders::_2))
	{
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
