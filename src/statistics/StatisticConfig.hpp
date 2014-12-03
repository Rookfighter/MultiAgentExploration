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
        std::string worldType;
        double coveredByObstacles;

        StatisticConfig()
                : simulation(NULL), coveredByObstacles(-1)
        {
        }
        ~StatisticConfig()
        {
        }

    };

}

#endif
