#ifndef MAE_ODOMETRY_HPP
#define MAE_ODOMETRY_HPP

namespace mae
{
	
	class Pose
	{
	public:
		double x;
		double y;
		double yaw;
	
		Pose();
		Pose(const double p_x, const double p_y, const double p_yaw);
		~Pose();
		
		
	};
	
	class Velocity
	{
	public:
		double linear;
		double angular;
		
		Velocity();
		Velocity(const double p_linear, const double p_angular);
		~Velocity();
	};

	class Odometry
	{
	public:
		Pose pose;
		Velocity velocity;
		
		Odometry();
		Odometry(const Pose &p_pose, const Velocity &p_velocity);
		~Odometry();

	};

}

#endif
