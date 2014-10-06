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
	
	Velocity::Velocity()
	:Velocity(0,0)
	{	}
	
	Velocity::Velocity(const double p_linear, const double p_angular)
	:linear(p_linear), angular(p_angular)
	{	}
	
	Velocity::~Velocity()
	{	}

	Odometry::Odometry()
	:Odometry(Pose(), Velocity())
	{	}
	
	Odometry::Odometry(const Pose &p_pose, const Velocity &p_velocity)
	:pose(p_pose), velocity(p_velocity)
	{	}

	Odometry::~Odometry()
	{	}


}
