#ifndef MAE_ROBOT_CONFIG_HPP
#define MAE_ROBOT_CONFIG_HPP

#include <string>
#include <stage.hh>
#include "simulation/MarkerStock.hpp"
#include "utils/Odometry.hpp"

namespace mae
{

    class RobotConfig
    {
    public:
        Stg::World *world;
        MarkerStock *stock;

        std::string name;
        int rangerIndex;

        Velocity maxVelocity;
        Velocity minVelocity;

        double markerSensorMaxRange;

        RobotConfig()
                : world(NULL), stock(NULL), rangerIndex(-1), markerSensorMaxRange(
                        1.0)
        {
        }
        ~RobotConfig()
        {
        }
    };

}

#endif
