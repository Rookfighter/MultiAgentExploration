#include "control/ExplorationBot.hpp"

namespace mae
{

	ExplorationBot::ExplorationBot(const ExplorationBotConfiguration &p_config)
	:name_(p_config.name), simulation_(p_config.simulation), motor_(simulation_->getClient(), p_config.motorIndex)
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
	
	void ExplorationBot::placeMarker()
	{
		simulation_->placeMarkerAt(getAbsolutePose());
	}

}
