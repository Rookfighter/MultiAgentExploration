#ifndef MAE_EXPERIMENT_TERMINATION_HPP
#define MAE_EXPERIMENT_TERMINATION_HPP

#include "algorithm/ExperimentConfig.hpp"

namespace mae
{

    class ExperimentTermination
    {
    private:
        Simulation *simulation_;
        Statistic *statistic_;
        int terminationMinutes_;
        double terminationCoverage_;
    public:
        ExperimentTermination(const ExperimentConfig &p_config);
        ~ExperimentTermination();

        bool terminated() const;
    };

}

#endif
