#include "algorithm-rt/Wagner.hpp"

namespace mae
{

	Wagner::Wagner(const AlgorithmConfig &p_config)
		:AntAlgorithm(p_config,
                      std::bind(&Wagner::updateValue, this, std::placeholders::_1, std::placeholders::_2),
                      std::bind(&Wagner::calcValue, this, std::placeholders::_1, std::placeholders::_2))
	{
	}

	Wagner::~Wagner()
	{
	}

	void Wagner::updateValue(Marker *p_current, Marker *p_next)
	{
		assert(p_current != NULL);
		
		// only update value if current is lower than next
		if(p_next != NULL && p_current->getValue() < calcValue(p_current, p_next))
			p_current->setValue(calcValue(p_current, p_next));
			
	}

	double Wagner::calcValue(Marker *p_current, Marker *p_next)
	{
		assert(p_next != NULL);
		
		double distance;
		if(p_current == NULL)
			distance = 1.0;
		else
			distance = (p_current->getPose().position - p_next->getPose().position).length();
		
		return  p_next->getValue() + distance;
	}


}
