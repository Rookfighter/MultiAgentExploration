#ifndef MAE_ALGORITHM_CONFIG_HPP
#define MAE_ALGORITHM_CONFIG_HPP

#include "simulation/ExplorationBot.hpp"

namespace mae
{
	enum AlgortihmType { NODECOUNTING };
	
	class AlgorithmConfig
	{
	public:

		ExplorationBot *robot;
		Simulation *simulation;
		MarkerStock *stock;
		
		AlgortihmType type;
		
		double obstacleAvoidDistance;
		double obstacleMarkerDistance;
		
		AlgorithmConfig() {
		}
		~AlgorithmConfig() {
		}

	};

}

#endif // MAE_ALGORITHM_CONFIG_HPP
