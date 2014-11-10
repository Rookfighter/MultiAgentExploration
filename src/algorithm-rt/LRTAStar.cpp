#include "algorithm-rt/LRTAStar.hpp"
#include "algorithm-rt/InitialAntState.hpp"

namespace mae
{


	LRTAStar::LRTAStar(const AlgorithmConfig &p_config)
		:AntAlgorithm()
	{
		AntStateProperties properties;
		properties.robot = p_config.robot;
		properties.stock = p_config.stock;
		properties.updateValue = std::bind(&LRTAStar::updateValue, this, std::placeholders::_1, std::placeholders::_2);
		properties.calcValue = std::bind(&LRTAStar::calcValue, this, std::placeholders::_1, std::placeholders::_2);
		properties.obstacleAvoidDistance = p_config.obstacleAvoidDistance;
		properties.obstacleMarkerDistance = p_config.obstacleMarkerDistance;

		init(new InitialAntState(properties));
	}

	LRTAStar::~LRTAStar()
	{
	}
	
	double LRTAStar::calcValue(Marker *p_current, Marker *p_next)
	{
		assert(p_next != NULL);
		
		double distance;
		if(p_current == NULL)
			distance = 1.0;
		else
			distance = (p_current->getPose().position - p_next->getPose().position).length();
		
		return  p_next->getValue() + distance;
	}

	void LRTAStar::updateValue(Marker *p_current, Marker *p_next)
	{
		assert(p_current != NULL);

		if(p_next == NULL)
			p_current->setValue(1.0);
		else
			p_current->setValue(calcValue(p_current, p_next));
	}


}
