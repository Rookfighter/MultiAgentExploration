#ifndef MAE_EXPERIMENT_HPP
#define MAE_EXPERIMENT_HPP

#include "algorithm/Algorithm.hpp"
#include "simulation/Simulation.hpp"
#include "utils/StopWatch.hpp"

namespace mae
{

	class Experiment
	{
		friend class ExperimentLoader;
	private:
		Simulation *simulation_;
		std::vector<Algorithm*> algorithms_;
		StopWatch algorithmWatch_;
		
		Experiment();
	public:
		~Experiment();
		
		void update();
		
		Algorithm* getAlgorithmOf(const std::string &p_robotName);
		std::vector<Algorithm*> getAlgorithms();
		
		Simulation* getSimulation();
	};

}

#endif
