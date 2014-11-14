#include <easylogging++.h>
#include "statistics/Statistic.hpp"

namespace mae
{

    Statistic::Statistic(const StatisticConfig &p_config)
        :experiment_(p_config.experiment),
         statisticGrid_(p_config)
    {
        LOG(INFO) << "Initialized Statistic";
    }

    Statistic::~Statistic()
    {
    }
    
    Experiment* Statistic::getExperiment()
    {
        return experiment_;
    }
    
    void Statistic::saveToDirectory(const std::string &p_directory)
    {

    }

    void Statistic::printLog()
    {

    }

    void Statistic::update()
    {
        statisticGrid_.update();
    }
}
