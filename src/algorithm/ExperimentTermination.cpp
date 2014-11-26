#include "algorithm/ExperimentTermination.hpp"
#include "utils/Convert.hpp"

namespace mae
{

    ExperimentTermination::ExperimentTermination(const ExperimentConfig &p_config)
            : simulation_(p_config.simulation), terminationMinutes_(p_config.terminationMinutes)
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

        return false;
    }

}
