#include <sstream>
#include "simulation/Marker.hpp"

namespace mae
{
	std::string const boolToString(bool b)
	{
		return b ? "true" : "false";
	}

	Marker::Marker(const double p_range, const int p_id)
		:id_(p_id), range_(p_range),
		 inUse_(false), value_(0), drawRange_(true)
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
	
	void Marker::setDrawRange(const bool p_drawRange)
	{
		drawRange_ = p_drawRange;
		redraw();
	}

	int Marker::getID() const
	{
		return id_;
	}
	
	const Pose& Marker::getPose() const
	{
		return pose_;
	}
	
	double Marker::getRange() const
	{
		return range_;
	}
	
	int Marker::getValue() const
	{
		return value_;
	}

	bool Marker::isInUse() const
	{
		return inUse_;
	}
	
	bool Marker::drawRange() const
	{
		return drawRange_;
	}
	
	void Marker::incrementValue()
	{
		value_++;
	}

	std::string Marker::str()
	{
		std::stringstream ss;
		ss.precision(2);
		ss << "id: " << id_ << " pos=" << pose_.str() << ",rng=" << range_ << ",use=" << boolToString(inUse_) << ",val=" << value_;

		return ss.str();
	}
	
	void Marker::redraw()
	{
		notifyAll(NULL);
	}
}
