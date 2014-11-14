#ifndef MAE_ODOMETRY_HPP
#define MAE_ODOMETRY_HPP

#include <string>
#include <stage.hh>
#include "utils/Vector2f.hpp"

namespace mae
{
	class Pose
	{
	public:
		Vector2f position;
		double yaw;
	
		Pose();
		Pose(const double p_x, const double p_y, const double p_yaw);
		~Pose();
		
        void fromStagePose(const Stg::Pose &p_pose);
        Stg::Pose asStagePose() const;
		void set(const double p_x, const double p_y, const double p_yaw);
		std::string str() const;
		
	};
	
	class Velocity
	{
	public:
		double linear;
		double angular;
		
		Velocity();
		Velocity(const double p_linear, const double p_angular);
		~Velocity();
		
		void set(const double p_linear, const double p_angular);
		std::string str() const;
	};

	class Odometry
	{
	public:
		Pose pose;
		Velocity velocity;
		
		Odometry();
		Odometry(const Pose &p_pose, const Velocity &p_velocity);
		~Odometry();
		
		std::string str() const;
	};

}

#endif
