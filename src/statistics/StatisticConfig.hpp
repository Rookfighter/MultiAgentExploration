#ifndef MAE_STATISTIC_CONFIG_HPP
#define MAE_STATISTIC_CONFIG_HPP

#include "algorithm/Experiment.hpp"

namespace mae
{

    class StatisticConfig
    {
    public:
        Experiment *experiment;
        Vector2 tileSize;
        Vector2 gridSize;
    
        StatisticConfig()
        { }
        ~StatisticConfig()
        { }

    };

}

#endif
