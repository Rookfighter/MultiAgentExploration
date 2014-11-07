#ifndef MAE_ODOMETRY_HPP
#define MAE_ODOMETRY_HPP

#include <string>

namespace mae
{
	class Vector2
	{
	public:
		double x;
		double y;
	
		Vector2(): x(0), y(0) { }
		Vector2(double p_x, double p_y): x(p_x), y(p_y) { }
		~Vector2() { }
		
		void set(const double p_x, const double p_y);
		double lengthSQ() const;
		double length() const;
		Vector2 perpendicular() const;
		
		Vector2& operator+=(Vector2 const& p_vec);
		Vector2& operator-=(Vector2 const& p_vec);
		Vector2& operator*=(const double p_factor);
		Vector2& operator/=(const double p_divisor);
		
		std::string str() const;
	};
	
	const Vector2 operator+(Vector2 const& p_vec1, Vector2 const& p_vec2);
	const Vector2 operator-(Vector2 const& p_vec1, Vector2 const& p_vec2);
	const Vector2 operator*(Vector2 const& p_vec, const double p_factor);
	const Vector2 operator*(const double p_factor, Vector2 const& p_vec);
	const Vector2 operator/(Vector2 const& p_vec, const double p_divisor);
	bool operator==(Vector2 const& p_vec1, Vector2 const& p_vec2);
	bool operator!=(Vector2 const& p_vec1, Vector2 const& p_vec2);
	
	class Pose
	{
	public:
		Vector2 position;
		double yaw;
	
		Pose();
		Pose(const double p_x, const double p_y, const double p_yaw);
		~Pose();
		
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
