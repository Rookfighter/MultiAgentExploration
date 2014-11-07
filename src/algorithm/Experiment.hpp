#ifndef MAE_EXPERIMENT_HPP
#define MAE_EXPERIMENT_HPP

#include "algorithm/Algorithm.hpp"
#include "simulation/World.hpp"
#include "utils/StopWatch.hpp"

namespace mae
{

	class Experiment
	{
		friend class ExperimentLoader;
	private:
		World *world_;
		std::vector<Algorithm*> algorithms_;
		StopWatch updateWatch_;
		StopWatch algorithmWatch_;
		
		Experiment();
	public:
		~Experiment();
		
		void step();
		
		Algorithm* getAlgorithmOf(const std::string &p_robotName);
		std::vector<Algorithm*> getAlgorithms();
		
		World* getWorld();
	};

}

#endif
