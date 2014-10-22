#include <sstream>
#include "simulation/Marker.hpp"

namespace mae
{
	std::string const boolToString(bool b)
	{
		return b ? "true" : "false";
	}

	Marker::Marker(const StockConfig& p_config, const int p_id)
		:simulation_(p_config.simulation) id_(p_id), range_(p_config.markerRange),
		 inUse_(false), value_(0)
	{
		std::stringstream ss;
		ss << p_config.markerName << id_;
		name_ = ss.str();
	}
	
	Marker::~Marker()
	{
		
	}

	void Marker::setPose(const Pose& p_pose)
	{
		pose_ = p_pose;
		simulation_->setPoseOf(name_, pose_);
	}
	
	void Marker::setValue(const int p_value)
	{
		value_ = p_value;
	}
	
	void Marker::setInUSe(const bool p_inUse)
	{
		inUse_ = p_inUse;
	}

	int Marker::getID() const
	{
		return id_;
	}
	
	const std::string& Marker::getName() const
	{
		return name_;
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

	void Marker::refreshData()
	{
		pose_ = simulation_->getPoseOf(name_);
	}

	std::string Marker::str()
	{
		std::stringstream ss;
		ss.precision(2);
		ss << "id: " << id_ << " " << name_ << ",pos=" << pose_.str() << ",rng=" << range_ << ",use=" << boolToString(inUse_) << ",val=" << value_;

		return ss.str();
	}
}
