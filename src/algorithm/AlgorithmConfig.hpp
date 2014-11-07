#ifndef MAE_ALGORITHM_CONFIG_HPP
#define MAE_ALGORITHM_CONFIG_HPP

#include "simulation/ExplorationBot.hpp"

namespace mae
{
	class AlgorithmConfig
	{
	public:

		ExplorationBot *robot;
		Simulation *simulation;
		MarkerStock *stock;
		
		std::string type;
		
		double obstacleAvoidDistance;
		double obstacleMarkerDistance;
		
		AlgorithmConfig() {
		}
		~AlgorithmConfig() {
		}

	};

}

#endif
