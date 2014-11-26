#ifndef MAE_EXPERIMENT_LOADER_HPP
#define MAE_EXPERIMENT_LOADER_HPP

#include "loading/SimulationLoader.hpp"
#include "loading/StatisticLoader.hpp"
#include "algorithm/Experiment.hpp"

namespace mae
{

	class ExperimentLoader
	{
	private:
	    AlgorithmConfig algorithmConfig_;
        ExperimentConfig experimentConfig_;

	    SimulationLoader simulationLoader_;
	    StatisticLoader statisticLoader_;
	public:
	    ExperimentLoader();
        ~ExperimentLoader();

		void load(const std::string &p_file);
        void load(YAML::Node &p_root);

        Experiment* create();
	};

}

#endif
