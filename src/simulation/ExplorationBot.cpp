#include <sstream>
#include <easylogging++.h>
#include "simulation/ExplorationBot.hpp"
#include "utils/Math.hpp"

namespace mae
{
	ExplorationBot::ExplorationBot(const RobotConfig &p_config)
		: name_(p_config.name),
		  motor_(p_config), ranger_(p_config), markerSensor_(p_config),
		  stock_(p_config.stock)
	{
		position_ = p_config.world->GetModel(name_);
		LOG(DEBUG) << "Initialized Robot (" << name_ << ")";
	}

	ExplorationBot::~ExplorationBot()
	{
	}

	void ExplorationBot::setPose(const Pose &p_pose)
	{
		position_.SetGlobalPose(Stg::Pose(p_pose.position.x, p_pose.position.y, 0, p_pose.yaw));
	}

	Pose ExplorationBot::getAbsolutePose()
	{
		Stg::Pose pose = position_.GetGlobalPose();
		return Pose(pose.x, pose.y, pose.yaw);
	}

	Motor& ExplorationBot::getMotor()
	{
		return motor_;
	}

	Ranger& ExplorationBot::getRanger()
	{
		return ranger_;
	}
	
	MarkerSensor& ExplorationBot::getMarkerSensor()
	{
		return markerSensor_;
	}

	std::string ExplorationBot::getName() const
	{
		return name_;
	}

	Marker* ExplorationBot::dropMarker()
	{
		Pose markerPose = getAbsolutePose();
		markerPose.yaw = 0;
		Marker *marker = stock_->acquireMarker();
		marker->setPose(markerPose);
		
		return marker;
	}
	
	void ExplorationBot::update()
	{
		markerSensor_.setRobotPose(getAbsolutePose());
	}
}
