#include "algorithm-rt/Wagner.hpp"
#include "algorithm-rt/InitialAntState.hpp"

namespace mae
{

	Wagner::Wagner(const AlgorithmConfig &p_config)
		:AntAlgorithm()
	{
		AntStateProperties properties;
		properties.robot = p_config.robot;
		properties.stock = p_config.stock;
		properties.updateValue = std::bind(&Wagner::updateValue, this, std::placeholders::_1, std::placeholders::_2);
		properties.calcValue = std::bind(&Wagner::calcValue, this, std::placeholders::_1, std::placeholders::_2);
		properties.obstacleAvoidDistance = p_config.obstacleAvoidDistance;
		properties.obstacleMarkerDistance = p_config.obstacleMarkerDistance;

		init(new InitialAntState(properties));
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
