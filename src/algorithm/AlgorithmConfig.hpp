#ifndef MAE_ALGORITHM_CONFIG_HPP
#define MAE_ALGORITHM_CONFIG_HPP

#include "simulation/ExplorationBot.hpp"

namespace mae
{
    class AlgorithmConfig
    {
    public:

        ExplorationBot *robot;
        MarkerStock *stock;

        std::string type;

        double obstacleStopDistance;
        double obstacleAvoidDistance;
        double obstacleMarkerDistance;
        double markerDeployDistance;
        double collisionResolveDistance;
        double markerTooCloseDistance;

        AlgorithmConfig()
                : robot(NULL),
                  stock(NULL),
                  obstacleStopDistance(-1),
                  obstacleAvoidDistance(-1),
                  obstacleMarkerDistance(-1),
                  markerDeployDistance(-1),
                  collisionResolveDistance(-1),
                  markerTooCloseDistance(-1)
        {
        }
        ~AlgorithmConfig()
        {
        }

    };

}

#endif
