#include <sstream>
#include "control/ExplorationBot.hpp"

namespace mae
{
	RangerConfig::RangerConfig(const int p_sensorCount)
		: count(p_sensorCount), sensorPose(count), fov(count), maxRange(count)
	{

	}
	
	std::string RangerConfig::str(const int p_index) const
	{
		std::stringstream ss;
		ss.precision(2);
		
		ss << "id=" << p_index << ",p=" << sensorPose[p_index].str() << ",fov=" << fov[p_index] << ",mr=" << maxRange[p_index];
		
		return ss.str();
	}

	ExplorationBot::ExplorationBot(PlayerClient *p_client,
	                               Simulation *p_simulation,
	                               const std::string &p_name,
	                               const int p_motorIndex)
		:client_(p_client), simulation_(p_simulation), motor_(client_->getClient(), p_motorIndex), name_(p_name)
	{
		motor_.SetMotorEnable(true);
		motor_.RequestGeom();
	}

	ExplorationBot::~ExplorationBot()
	{
		motor_.SetMotorEnable(false);
	}

	void ExplorationBot::setPose(const Pose &p_pose)
	{
		simulation_->setPoseOf(name_, p_pose);
	}

	Pose ExplorationBot::getPose() const
	{
		Pose result;
		result.x = motor_.GetXPos();
		result.y = motor_.GetYPos();
		result.yaw = motor_.GetYaw();
		return result;
	}

	Pose ExplorationBot::getAbsolutePose()
	{
		return simulation_->getPoseOf(name_);
	}

	void ExplorationBot::setVelocity(const Velocity &p_velocity)
	{
		motor_.SetSpeed(p_velocity.linear, p_velocity.angular);
	}

	Velocity ExplorationBot::getVelocity() const
	{
		Velocity result;
		result.linear = motor_.GetXSpeed();
		result.angular = motor_.GetYawSpeed();
	}

	std::string ExplorationBot::getName() const
	{
		return name_;
	}

	void ExplorationBot::placeMarker(Marker* p_marker)
	{
		Pose currentPose = getAbsolutePose();
		currentPose.yaw = 0;
		p_marker->pose = currentPose;
		simulation_->setPoseOf(p_marker->name, currentPose);
	}

}
