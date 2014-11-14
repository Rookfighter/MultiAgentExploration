#include <sstream>
#include <cmath>
#include "utils/Odometry.hpp"

namespace mae
{
	
	
	Pose::Pose()
	:Pose(0,0,0)
	{	}
	
	Pose::Pose(const double p_x, const double p_y, const double p_yaw)
	:position(p_x, p_y), yaw(p_yaw)
	{	}
	
	Pose::~Pose()
	{	}
	
    void Pose::fromStagePose(const Stg::Pose &p_pose)
    {
        position.x = p_pose.x;
        position.y = p_pose.y;
        yaw = p_pose.a;
    }
    
    Stg::Pose Pose::asStagePose() const
    {
        return Stg::Pose(position.x, position.y, 0, yaw);
        
    }
    
	void Pose::set(const double p_x, const double p_y, const double p_yaw)
	{
		position.set(p_x, p_y);
		yaw = p_yaw;
	}
	
	std::string Pose::str() const
	{
		std::stringstream ss;
		ss.precision(2);
		ss << "(" << position.x << "/" << position.y << "/" << yaw << ")";
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
