#ifndef MAE_ROBOT_CONFIG_HPP
#define MAE_ROBOT_CONFIG_HPP

#include <string>
#include <stage.hh>
#include "simulation/Simulation.hpp"
#include "simulation/MarkerStock.hpp"
#include "utils/Odometry.hpp"

namespace mae
{

	class RobotConfig
	{
	public:
		Stg::World *world;
		Simulation *simulation;
		MarkerStock *stock;
		
		std::string name;
		int motorIndex;
		int rangerIndex;
		int graphicsIndex;
		
		Velocity maxVelocity;
		Velocity minVelocity;
		
		double markerSensorMaxRange;
		
		RobotConfig() { }
		~RobotConfig() { }
	};

}

#endif
