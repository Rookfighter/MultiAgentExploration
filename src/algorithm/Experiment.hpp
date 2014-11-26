#ifndef MAE_EXPERIMENT_HPP
#define MAE_EXPERIMENT_HPP

#include "algorithm/ExperimentConfig.hpp"
#include "algorithm/ExperimentTermination.hpp"
#include "utils/StopWatch.hpp"

namespace mae
{
	class Experiment
	{
		friend class ExperimentLoader;
	private:
		Simulation *simulation_;
		std::vector<Algorithm*> algorithms_;
		ExperimentTermination experimentTermination_;
		StopWatch algorithmWatch_;
		
		Experiment(const ExperimentConfig &p_config);
	public:
		~Experiment();
		
		void update();
		bool terminated() const;
		
		Algorithm* getAlgorithmOf(const std::string &p_robotName);
		std::vector<Algorithm*> getAlgorithms();
		
		Simulation* getSimulation();
	};

}

#endif
