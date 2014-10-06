#include "Meybot.hpp"

namespace tut
{

	Meybot::Meybot()
	:Meybot("localhost", 6665)
	{
	}
	
	Meybot::Meybot(const std::string &p_host, const int p_port)
	: robot_(p_host, p_port), motor_(&robot_, 0), rangers_(&robot_, 0)
	{
		motor_.SetMotorEnable(true);
		
		motor_.RequestGeom();
		rangers_.RequestGeom();
	}

	Meybot::~Meybot()
	{
		
	}
	
	void Meybot::setSpeed(const double p_linVel, const double p_angVel)
	{
		motor_.SetSpeed(p_linVel, p_angVel);
	}
	
	double Meybot::getLinearVelocity() const
	{
		return motor_.GetXSpeed();
	}
	
	double Meybot::getAngularVelocity() const
	{
		return motor_.GetYawSpeed();
	}
	
	Pose Meybot::getPose() const
	{
		Pose result;
		result.x = motor_.GetXPos();
		result.y = motor_.GetYPos();
		result.yaw = motor_.GetYaw();
		return result;
	}
	
	double Meybot::getSonarDistance(const int p_sensor) const
	{
		return rangers_[p_sensor];
	}
	
	int Meybot::getSonarCount() const
	{
		return 3;
	}
	
	void Meybot::update()
	{
		robot_.Read();
	}
}
