#include <sstream>
#include <cmath>
#include "utils/Odometry.hpp"

namespace mae
{
	void Vector2::set(const double p_x, const double p_y)
	{
		x = p_x;
		y = p_y;
	}
	
	double Vector2::lengthSQ() const
	{
		return x * x + y * y;
	}
	
	double Vector2::length() const
	{
		return sqrt(lengthSQ());
	}
	
	Vector2 Vector2::perpendicular() const
	{
		return Vector2(-y, x);
	}
	
	Vector2& Vector2::operator+=(Vector2 const& p_vec)
	{
		x += p_vec.x;
		y += p_vec.y;
		
		return *this;
	}
	
	Vector2& Vector2::operator-=(Vector2 const& p_vec)
	{
		x -= p_vec.x;
		y -= p_vec.y;
		
		return *this;
	}
	
	Vector2& Vector2::operator*=(const double p_factor)
	{
		x *= p_factor;
		y *= p_factor;
		
		return *this;
	}
	
	Vector2& Vector2::operator/=(const double p_divisor)
	{
		x /= p_divisor;
		y /= p_divisor;
		
		return *this;
	}
	
	std::string Vector2::str() const
	{
		std::stringstream ss;
		ss.precision(2);
		ss << "(" << x << ";" << y << ")";
		return ss.str();
	}
	
	const Vector2 operator+(Vector2 const& p_vec1, Vector2 const& p_vec2)
	{
		Vector2 result(p_vec1);
		result += p_vec2;
		
		return result;
	}
	
	const Vector2 operator-(Vector2 const& p_vec1, Vector2 const& p_vec2)
	{
		Vector2 result(p_vec1);
		result -= p_vec2;
		
		return result;
	}
	
	const Vector2 operator*(Vector2 const& p_vec, const double p_factor)
	{
		Vector2 result(p_vec);
		result *= p_factor;
		
		return result;
	}
	
	const Vector2 operator*(const double p_factor, Vector2 const& p_vec)
	{
		return p_vec * p_factor;
	}
	
	const Vector2 operator/(Vector2 const& p_vec, const double p_divisor)
	{
		Vector2 result(p_vec);
		result /= p_divisor;
		
		return result;
	}
	
	bool operator==(Vector2 const& p_vec1, Vector2 const& p_vec2)
	{
		return p_vec1.x == p_vec2.x && p_vec1.y == p_vec2.y;
	}
	
	bool operator!=(Vector2 const& p_vec1, Vector2 const& p_vec2)
	{
		return !(p_vec1 == p_vec2);
	}
	
	Pose::Pose()
	:Pose(0,0,0)
	{	}
	
	Pose::Pose(const double p_x, const double p_y, const double p_yaw)
	:position(p_x, p_y), yaw(p_yaw)
	{	}
	
	Pose::~Pose()
	{	}
	
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
