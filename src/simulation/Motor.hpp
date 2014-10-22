#ifndef MAE_MOTOR_HPP
#define MAE_MOTOR_HPP

#include "simulation/RobotConfig.hpp"

namespace mae
{

	class Motor
	{
	private:
		PlayerCc::Position2dProxy motor_;
		
		Velocity maxVelocity_;
		Velocity minVelocity_;
	public:
		Motor(const RobotConfig &p_config);
		~Motor();
		
		Pose getPose() const;
		
		void setVelocity(const Velocity &p_velocity);
		Velocity getVelocity() const;
		
		const Velocity& getMaxVelocity() const;
		const Velocity& getMinVelocity() const;
	};

}

#endif
