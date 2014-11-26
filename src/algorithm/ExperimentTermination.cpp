#include "algorithm/ExperimentTermination.hpp"
#include "utils/Convert.hpp"

namespace mae
{

    ExperimentTermination::ExperimentTermination(const ExperimentConfig &p_config)
            : simulation_(p_config.simulation),
              statistic_(p_config.statistic),
              terminationMinutes_(p_config.terminationMinutes),
              terminationCoverage_(p_config.terminationCoverage)
    {

    }

    ExperimentTermination::~ExperimentTermination()
    {
    }

    bool ExperimentTermination::terminated() const
    {
        int currentMinutes = usecToMin(simulation_->getWorld()->SimTimeNow());
        if(terminationMinutes_ > 0 && terminationMinutes_ <= currentMinutes)
            return true;

        if(terminationCoverage_ > 0 && terminationCoverage_ <= statistic_->getStatisticGrid().getCoverage())
            return true;

        return false;
    }

}
