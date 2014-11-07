#include <sstream>
#include <easylogging++.h>
#include "simulation/ExplorationBot.hpp"
#include "utils/Math.hpp"

namespace mae
{
	ExplorationBot::ExplorationBot(const RobotConfig &p_config)
		: name_(p_config.name), simulation_(p_config.simulation),
		  motor_(p_config), ranger_(p_config), markerSensor_(p_config),
		  stock_(p_config.stock)
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
		return absolutePose_;
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
		absolutePose_ = simulation_->getPoseOf(name_);
		markerSensor_.setRobotPose(absolutePose_);
	}
}
