#include <sstream>
#include "simulation/Marker.hpp"
#include "utils/Convert.hpp"

namespace mae
{
	Marker::Marker(const int p_id)
		:id_(p_id),model_()
		value_(0), highlighted_(false)
	{
		model_.AddBlockRect(0, 0, 0.05, 0.05, 0.01);
		model_.SetColor(Stg::Color.red);
		
		model_.SetBlobReturn(false);
		model_.SetGravityReturn(false);
		model_.SetGripperReturn(false);
		model_.SetObstacleReturn(false);
		model_.SetRangerReturn(false);
		model_.SetFiducialReturn(false);
		model_.SetStickyReturn(false);
	}
	
	Marker::~Marker()
	{
	}

	void Marker::connect(Stg::World* p_world)
	{
		p_world.AddModel(&model_);
	}
	
	void Marker::disconnect(Stg::World* p_world)
	{
		p_world.RemoveModel(&model_);
	}

	void Marker::setPose(const Pose& p_pose)
	{
		model_.SetGlobalPose(p_pose.position.x, pose.position.y, 0, pose.yaw);
	}
	
	void Marker::setValue(const int p_value)
	{
		value_ = p_value;
	}
	
	void Marker::setHighlighted(const bool p_highlighted)
	{
		highlighted_ = p_highlighted;
		if(highlighted_)
			model_.SetColor(Stg::Color.green);
		else
			model_.SetColor(Stg::Color.red);
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
		Stg::Pose pose = model_.GetGlobalPose();
		return Pose(pose.x, pose.y, pose.a);
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
		ss << "id: " << id_ << " pos=" << getPose().str() << ",val=" << value_;

		return ss.str();
	}
}
