#include <easylogging++.h>
#include "control/Pioneer2DX.hpp"

#define P2DX_MAX_LIN_VELOCITY 2.0
#define P2DX_MAX_ANG_VELOCITY 2.0

namespace mae
{

	Pioneer2DX::Pioneer2DX(PlayerClient *p_client,
	                       Simulation *p_simulation,
	                       const std::string &p_name,
	                       const int p_motorIndex,
						   const int p_rangerIndex)
		:ExplorationBot(p_client, p_simulation, p_name, p_motorIndex),
		 ranger_(client_->getClient(), p_rangerIndex), config_()
	{
		LOG(DEBUG) << "Connected RangerProxy: " << p_rangerIndex << " (" << name_ << ")";
		
		ranger_.RequestGeom();
		ranger_.RequestConfigure();
		
		LOG(DEBUG) << "Requested Geometry for RangerProxy (" << name_ << ")";
		
		initConfig();
		
		LOG(DEBUG) << "Initialized RangerConfig (" << name_ << ")";
	}
	
	void Pioneer2DX::initConfig()
	{
		config_.setSensorCount(ranger_.GetElementCount());

		double maxRange = ranger_.GetMaxRange();
		double fov = ranger_.GetMaxAngle() - ranger_.GetMinAngle();

		for(int i = 0; i < config_.sensorCount; ++i) {
			player_pose3d_t pose = ranger_.GetElementPose(i);

			config_.sensorPose[i].set(pose.px, pose.py, pose.pyaw);
			config_.maxRange[i] = maxRange;
			config_.fov[i] = fov;
		}
	}

	Pioneer2DX::~Pioneer2DX()
	{
	}

	RangerConfig Pioneer2DX::getRangerConfig()
	{
		return config_;
	}

	int Pioneer2DX::getRangerCount()
	{
		return config_.sensorCount;
	}
	double Pioneer2DX::getRangerDistance(const int p_index)
	{
		return ranger_[p_index];
	}
	
	bool Pioneer2DX::hasValidRangerData()
	{
		return ranger_.IsValid();
	}

	Velocity Pioneer2DX::getMaxVelocity() const
	{
		return Velocity(P2DX_MAX_LIN_VELOCITY, P2DX_MAX_ANG_VELOCITY);
	}
}
