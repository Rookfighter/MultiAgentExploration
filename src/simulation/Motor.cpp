#include <easylogging++.h>
#include "simulation/Motor.hpp"

namespace mae
{

	Motor::Motor(const RobotConfig &p_config)
	:motor_(NULL),
	 maxVelocity_(p_config.maxVelocity), minVelocity_(p_config.minVelocity)
	{
		motor_ = reinterpret_cast<Stg::ModelPosition*>(p_config.world->GetModel(p_config.name));
		motor_->Subscribe();
		LOG(DEBUG) << "Connected ModelPosition: " << p_config.name;
	}
	
	Motor::~Motor()
	{
		motor_->Unsubscribe();
	}

	Pose Motor::getPose() const
	{
		Pose result;
		result.position.x = motor_->est_pose.x;
		result.position.y = motor_->est_pose.y;
		result.yaw = motor_->est_pose.a;
		return result;
	}

	void Motor::setVelocity(const Velocity &p_velocity)
	{
		motor_->SetSpeed(p_velocity.linear, 0, p_velocity.angular);
	}
	
	Velocity Motor::getVelocity() const
	{
		Stg::Velocity velocity = motor_->GetVelocity();
		return Velocity(velocity.x, velocity.a);
	}
	
	void Motor::stop()
	{
		motor_->Stop();
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
