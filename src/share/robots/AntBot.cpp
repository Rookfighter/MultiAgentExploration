#include "AntBot.hpp"

namespace mae
{

	AntBot::AntBot(const std::string &p_name, const std::string &p_host, const int p_port)
	:name_(p_name), robot_(p_host, p_port), motor_(&robot_, 0), simulation_(&robot_,0)
	{
		motor_.SetMotorEnable(true);
		
		motor_.RequestGeom();
	}

	AntBot::~AntBot()
	{
		motor_.SetMotorEnable(false);
	}
	
	
	Pose AntBot::getPose() const
	{
		Pose result;
		result.x = motor_.GetXPos();
		result.y = motor_.GetYPos();
		result.yaw = motor_.GetYaw();
		return result;
	}
	
	Pose AntBot::getAbsolutePose()
	{
		Pose result;
		char tmpName[name_.length() + 1];
		strcpy(tmpName, name_.c_str());
		simulation_.GetPose2d(tmpName, result.x, result.y, result.yaw);
		return result;
	}
	
	void AntBot::setVelocity(const Velocity &p_velocity)
	{
		motor_.SetSpeed(p_velocity.linear, p_velocity.angular);
	}
	
	Velocity AntBot::getVelocity() const
	{
		Velocity result;
		result.linear = motor_.GetXSpeed();
		result.angular = motor_.GetYawSpeed();
	}
	
	std::string AntBot::getName() const
	{
		return name_;
	}
	
	void AntBot::update()
	{
		robot_.Read();
	}

}
