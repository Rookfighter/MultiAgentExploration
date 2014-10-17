#include <sstream>
#include <easylogging++.h>
#include "control/ExplorationBot.hpp"
#include "common/Math.hpp"

namespace mae
{
	/*=============
	 * RangerConfig
	 *=============*/
	RangerConfig::RangerConfig()
		:RangerConfig(0)
	{

	}

	RangerConfig::RangerConfig(const int p_sensorCount)
		: sensorCount(p_sensorCount), sensorPose(sensorCount), fov(sensorCount), maxRange(sensorCount)
	{
	}

	void RangerConfig::setSensorCount(const int p_sensorCount)
	{
		sensorCount = p_sensorCount;
		sensorPose.resize(sensorCount);
		fov.resize(sensorCount);
		maxRange.resize(sensorCount);
	}

	std::string RangerConfig::str(const int p_index) const
	{
		std::stringstream ss;
		ss.precision(2);

		ss << "id=" << p_index << ",p=" << sensorPose[p_index].str() << ",fov=" << fov[p_index] << ",mr=" << maxRange[p_index];

		return ss.str();
	}

	/*===============
	 * ExplorationBot
	 *===============*/

	ExplorationBot::ExplorationBot(const RobotConfig &p_config)
		:client_(p_config.client), simulation_(p_config.simulation),
		 motor_(client_->getClient(), p_config.motorIndex),
		 ranger_(client_->getClient(), p_config.rangerIndex),
		 name_(p_config.name), maxVelocity_(p_config.maxVelocity),
		 minVelocity_(p_config.minVelocity), config_()
	{
		LOG(DEBUG) << "Connected PositionProxy: " << p_config.motorIndex << " (" << name_ << ")";
		LOG(DEBUG) << "Connected RangerProxy: " << p_config.rangerIndex << " (" << name_ << ")";

		motor_.SetMotorEnable(true);
		motor_.RequestGeom();
		LOG(DEBUG) << "Requested Geometry PositionProxy (" << name_ << ")";

		ranger_.RequestGeom();
		ranger_.RequestConfigure();

		LOG(DEBUG) << "Requested Geometry for RangerProxy (" << name_ << ")";

		updateRangerConfig();

		LOG(DEBUG) << "Initialized RangerConfig (" << name_ << ")";
	}

	void ExplorationBot::updateRangerConfig()
	{
		config_.setSensorCount(ranger_.GetElementCount());
		config_.rangeCount = ranger_.GetRangeCount();

		double maxRange = ranger_.GetMaxRange();
		double fov = ranger_.GetMaxAngle() - ranger_.GetMinAngle();

		for(int i = 0; i < config_.sensorCount; ++i) {
			player_pose3d_t pose = ranger_.GetElementPose(i);

			config_.sensorPose[i].set(pose.px, pose.py, pose.pyaw);
			config_.maxRange[i] = maxRange;
			config_.fov[i] = fov;
		}
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
		result.position.x = motor_.GetXPos();
		result.position.y = motor_.GetYPos();
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

	double ExplorationBot::getRangerDistance(const int p_index)
	{
		return ranger_[p_index];
	}

	bool ExplorationBot::hasValidRangerData()
	{
		return ranger_.IsValid();
	}

	int ExplorationBot::getRangerCount() const
	{
		return config_.sensorCount;
	}

	const RangerConfig& ExplorationBot::getRangerConfig() const
	{
		return config_;
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

	Vector2 ExplorationBot::getDistanceTo(Marker* p_marker)
	{
		// TODO : implement distance to marker direction with some derivation
		Vector2 result;
		result = p_marker->pose.position - getAbsolutePose().position;
		return result;
	}
	
	double ExplorationBot::getAngleTo(Marker* p_marker)
	{
		Vector2 distance = getDistanceTo(p_marker);
		double result = atan2(distance.y, distance.x) - getAbsolutePose().yaw;
		result = normalizeRadian(result);
		return result;
	}
	
	Velocity ExplorationBot::getMaxVelocity() const
	{
		return maxVelocity_;
	}
	
	Velocity ExplorationBot::getMinVelocity() const
	{
		return minVelocity_;
	}
}
