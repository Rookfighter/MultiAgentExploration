#ifndef MAE_STATISTIC_CONFIG_HPP
#define MAE_STATISTIC_CONFIG_HPP

#include "simulation/Simulation.hpp"

namespace mae
{

    class StatisticConfig
    {
    public:
        Simulation *simulation;
        Vector2f tileSize;
        std::string floorplanName;
        std::vector<double> coverageEvents;
        std::string algorithmType;

        StatisticConfig()
                : simulation(NULL)
        {
        }
        ~StatisticConfig()
        {
        }

    };

}

#endif
