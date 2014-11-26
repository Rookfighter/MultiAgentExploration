#ifndef MAE_EXPERIMENT_CONFIG_HPP
#define MAE_EXPERIMENT_CONFIG_HPP

#include <vector>
#include "algorithm/Algorithm.hpp"
#include "statistics/Statistic.hpp"
#include "simulation/Simulation.hpp"

namespace mae
{
    class ExperimentConfig
    {
    public:
        Simulation *simulation;
        Statistic *statistic;
        std::vector<Algorithm*> algorithms;

        int terminationMinutes;
        double terminationCoverage;

        ExperimentConfig()
                : simulation(NULL),
                  statistic(NULL),
                  terminationMinutes(-1),
                  terminationCoverage(-1)
        {
        }

        ~ExperimentConfig()
        {
        }
    };
}

#endif
