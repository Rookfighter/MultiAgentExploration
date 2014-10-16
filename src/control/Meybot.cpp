#include <easylogging++.h>
#include "control/Meybot.hpp"
#include "common/Math.hpp"

#define MEYBOT_MAX_LIN_VELOCITY 2.0
#define MEYBOT_MAX_ANG_VELOCITY 2.0

#define MEYBOT_MIN_LIN_VELOCITY -MEYBOT_MAX_LIN_VELOCITY
#define MEYBOT_MIN_ANG_VELOCITY -MEYBOT_MAX_ANG_VELOCITY

namespace mae
{
	Meybot::Meybot(PlayerClient *p_client,
	               Simulation *p_simulation,
	               const std::string &p_name,
	               const int p_motorIndex,
	               const int p_rangerIndex)
		:ExplorationBot(p_client, p_simulation, p_name, p_motorIndex, p_rangerIndex)
	{

	}


	Meybot::~Meybot()
	{
	}

	Velocity Meybot::getMaxVelocity() const
	{
		return Velocity(MEYBOT_MAX_LIN_VELOCITY, MEYBOT_MAX_ANG_VELOCITY);
	}

	Velocity Meybot::getMinVelocity() const
	{
		return Velocity(MEYBOT_MIN_LIN_VELOCITY, MEYBOT_MIN_ANG_VELOCITY);
	}

}
