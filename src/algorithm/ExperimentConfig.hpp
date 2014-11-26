#ifndef MAE_EXPERIMENT_CONFIG_HPP
#define MAE_EXPERIMENT_CONFIG_HPP

#include <vector>
#include "algorithm/Algorithm.hpp"
#include "simulation/Simulation.hpp"

namespace mae
{
    class ExperimentConfig
    {
    public:
        Simulation *simulation;
        std::vector<Algorithm*> algorithms;

        int terminationMinutes;

        ExperimentConfig()
        { }

        ~ExperimentConfig()
        { }
    };
}

#endif
