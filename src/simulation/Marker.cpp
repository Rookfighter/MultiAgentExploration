#include <sstream>
#include "simulation/Marker.hpp"
#include "utils/Convert.hpp"

namespace mae
{
	Marker::Marker( const int p_id)
		:id_(p_id),
		value_(0), highlighted_(false)
	{
	}
	
	Marker::~Marker()
	{
	}

	void Marker::setPose(const Pose& p_pose)
	{
		pose_ = p_pose;
		redraw();
	}
	
	void Marker::setValue(const int p_value)
	{
		value_ = p_value;
	}
	
	void Marker::setHighlighted(const bool p_highlighted)
	{
		highlighted_ = p_highlighted;
		redraw();
	}
	
	bool Marker::isHighlighted() const
	{
		return highlighted_;
	}

	int Marker::getID() const
	{
		return id_;
	}
	
	const Pose& Marker::getPose() const
	{
		return pose_;
	}
	
	int Marker::getValue() const
	{
		return value_;
	}
	
	void Marker::incrementValue()
	{
		value_++;
	}

	std::string Marker::str()
	{
		std::stringstream ss;
		ss.precision(2);
		ss << "id: " << id_ << " pos=" << pose_.str() << ",val=" << value_;

		return ss.str();
	}
	
	void Marker::redraw()
	{
		notifyAll(NULL);
	}
}
