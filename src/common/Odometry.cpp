#include <sstream>
#include "common/Odometry.hpp"

namespace mae
{
	Pose::Pose()
	:Pose(0,0,0)
	{	}
	
	Pose::Pose(const double p_x, const double p_y, const double p_yaw)
	:x(p_x), y(p_y), yaw(p_yaw)
	{	}
	
	Pose::~Pose()
	{	}
	
	void Pose::set(const double p_x, const double p_y, const double p_yaw)
	{
		x = p_x;
		y = p_y;
		yaw = p_yaw;
	}
	
	std::string Pose::str() const
	{
		std::stringstream ss;
		ss.precision(2);
		ss << "(" << x << "/" << y << "/" << yaw << ")";
		return ss.str();
	}
	
	Velocity::Velocity()
	:Velocity(0,0)
	{	}
	
	Velocity::Velocity(const double p_linear, const double p_angular)
	:linear(p_linear), angular(p_angular)
	{	}
	
	Velocity::~Velocity()
	{	}
	
	void Velocity::set(const double p_linear, const double p_angular)
	{
		linear = p_linear;
		angular = p_angular;
	}
	
	std::string Velocity::str() const
	{
		std::stringstream ss;
		ss.precision(2);
		ss << "(" << linear << "/" << angular << ")";
		return ss.str();
	}

	Odometry::Odometry()
	:Odometry(Pose(), Velocity())
	{	}
	
	Odometry::Odometry(const Pose &p_pose, const Velocity &p_velocity)
	:pose(p_pose), velocity(p_velocity)
	{	}

	Odometry::~Odometry()
	{	}
	
	std::string Odometry::str() const
	{
		std::stringstream ss;
		ss << pose.str() << "," << velocity.str();
		return ss.str();
	}

}
