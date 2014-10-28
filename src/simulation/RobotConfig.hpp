#ifndef MAE_ROBOT_CONFIG_HPP
#define MAE_ROBOT_CONFIG_HPP

#include <string>
#include "simulation/Simulation.hpp"
#include "simulation/MarkerStock.hpp"
#include "common/Odometry.hpp"

namespace mae
{

	class RobotConfig
	{
	public:
		PlayerClient *client;
		Simulation *simulation;
		MarkerStock *stock;
		
		std::string name;
		int motorIndex;
		int rangerIndex;
		
		Velocity maxVelocity;
		Velocity minVelocity;
		
		RobotConfig();
		~RobotConfig();
	};

}

#endif
