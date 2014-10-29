#include <easylogging++.h>
#include "simulation/Motor.hpp"

namespace mae
{

	Motor::Motor(const RobotConfig &p_config)
	:motor_(p_config.client->getClient(), p_config.motorIndex),
	 maxVelocity_(p_config.maxVelocity), minVelocity_(p_config.minVelocity)
	{
		LOG(DEBUG) << "Connected PositionProxy: " << p_config.motorIndex << " (" << p_config.name << ")";
		
		motor_.SetMotorEnable(true);
		motor_.RequestGeom();
		LOG(DEBUG) << "Requested Geometry and enabled PositionProxy " << p_config.motorIndex << " (" << p_config.name << ")";
	}
	
	Motor::~Motor()
	{
		motor_.SetMotorEnable(false);
	}

	Pose Motor::getPose() const
	{
		Pose result;
		result.position.x = motor_.GetXPos();
		result.position.y = motor_.GetYPos();
		result.yaw = motor_.GetYaw();
		return result;
	}

	void Motor::setVelocity(const Velocity &p_velocity)
	{
		motor_.SetSpeed(p_velocity.linear, p_velocity.angular);
	}
	
	Velocity Motor::getVelocity() const
	{
		Velocity result;
		result.linear = motor_.GetXSpeed();
		result.angular = motor_.GetYawSpeed();
		return result;
	}
	
	void Motor::stop()
	{
		setVelocity(Velocity(0,0));
	}

	const Velocity& Motor::getMaxVelocity() const
	{
		return maxVelocity_;
	}
	
	const Velocity& Motor::getMinVelocity() const
	{
		return minVelocity_;
	}


}
