#include "control/Meybot.hpp"
#include "common/Math.hpp"

#define MEYBOT_RANGER_COUNT 3
#define MEYBOT_MAX_RANGE 3.0
#define MEYBOT_FOV 30

namespace mae
{
	Meybot::Meybot(PlayerClient *p_client,
	               Simulation *p_simulation,
	               const std::string &p_name,
	               const int p_motorIndex)
		:ExplorationBot(p_client, p_simulation, p_name, p_motorIndex), ranger_(client_->getClient(), 0)
	{
		ranger_.RequestGeom();
		ranger_.RequestConfigure();
	}

	Meybot::~Meybot()
	{

	}

	RangerConfig Meybot::getRangerConfig()
	{
		RangerConfig result(MEYBOT_RANGER_COUNT);
		
		double maxRange = ranger_.GetMaxRange();
		double fov = ranger_.GetMaxAngle() - ranger_.GetMinAngle();
		
		for(int i = 0; i < MEYBOT_RANGER_COUNT; ++i) {
			player_pose3d_t pose = ranger_.GetElementPose(i);
			
			result.sensorPose[i].set(pose.px, pose.py, pose.pyaw);
			result.maxRange[i] = maxRange;
			result.fov[i] = fov;
		}
		
		return result;
	}
	
	int Meybot::getRangerCount()
	{
		return MEYBOT_RANGER_COUNT;
	}

	double Meybot::getRangerDistance(const int p_index)
	{
		return ranger_[p_index];
	}

}
