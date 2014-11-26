#ifndef MAE_EXPERIMENT_TERMINATION_HPP
#define MAE_EXPERIMENT_TERMINATION_HPP

#include "algorithm/ExperimentConfig.hpp"
#include "simulation/Simulation.hpp"

namespace mae
{

    class ExperimentTermination
    {
    private:
        Simulation *simulation_;

        int terminationMinutes_;
    public:
        ExperimentTermination(const ExperimentConfig &p_config);
        ~ExperimentTermination();

        bool terminated() const;
    };

}

#endif
