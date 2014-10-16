#include <easylogging++.h>
#include "control/Pioneer2DX.hpp"

#define P2DX_MAX_LIN_VELOCITY 2.0
#define P2DX_MAX_ANG_VELOCITY 2.0

#define P2DX_MIN_LIN_VELOCITY -P2DX_MAX_LIN_VELOCITY
#define P2DX_MIN_ANG_VELOCITY -P2DX_MAX_ANG_VELOCITY

namespace mae
{

	Pioneer2DX::Pioneer2DX(PlayerClient *p_client,
	                       Simulation *p_simulation,
	                       const std::string &p_name,
	                       const int p_motorIndex,
	                       const int p_rangerIndex)
		:ExplorationBot(p_client, p_simulation, p_name, p_motorIndex, p_rangerIndex)
	{
	}

	Pioneer2DX::~Pioneer2DX()
	{
	}

	Velocity Pioneer2DX::getMaxVelocity() const
	{
		return Velocity(P2DX_MAX_LIN_VELOCITY, P2DX_MAX_ANG_VELOCITY);
	}

	Velocity Pioneer2DX::getMinVelocity() const
	{
		return Velocity(P2DX_MIN_LIN_VELOCITY, P2DX_MIN_ANG_VELOCITY);
	}
}
