#include "algorithm-rt/LRTAStar.hpp"
#include "algorithm-rt/InitialAntState.hpp"

namespace mae
{

	static double calcValueLRTAStar(Marker *p_current, Marker *p_next)
	{
		assert(p_next != NULL);

		return  p_next->getValue();
	}

	static void updateValueLRTAStar(Marker *p_current, Marker *p_next)
	{
		assert(p_current != NULL);

		if(p_next == NULL)
			p_current->setValue(1.0);
		else
			p_current->setValue(p_next->getValue() + 1.0);

	}

	LRTAStar::LRTAStar(const AlgorithmConfig &p_config)
		:AntAlgorithm()
	{
		AntStateProperties properties;
		properties.robot = p_config.robot;
		properties.stock = p_config.stock;
		properties.updateValue = updateValueLRTAStar;
		properties.calcValue = calcValueLRTAStar;
		properties.obstacleAvoidDistance = p_config.obstacleAvoidDistance;
		properties.obstacleMarkerDistance = p_config.obstacleMarkerDistance;

		init(new InitialAntState(properties));
	}

	LRTAStar::~LRTAStar()
	{
	}


}
