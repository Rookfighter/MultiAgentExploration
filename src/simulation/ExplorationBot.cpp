#include <sstream>
#include <easylogging++.h>
#include "simulation/ExplorationBot.hpp"
#include "common/Math.hpp"

namespace mae
{
	ExplorationBot::ExplorationBot(const RobotConfig &p_config)
		: name_(p_config.name), simulation_(p_config.simulation),
		  motor_(p_config), ranger_(p_config)
	{
		LOG(DEBUG) << "Initialized Robot (" << name_ << ")";
	}

	ExplorationBot::~ExplorationBot()
	{
	}

	void ExplorationBot::setPose(const Pose &p_pose)
	{
		simulation_->setPoseOf(name_, p_pose);
	}

	Pose ExplorationBot::getAbsolutePose()
	{
		return simulation_->getPoseOf(name_);
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

	void ExplorationBot::dropMarker()
	{
		Pose markerPose = getAbsolutePose();
		markerPose.yaw = 0;
		Marker *marker = stock_->acquireMarker();
		marker->setPose(markerPose)
		
		return marker;
	}
}
