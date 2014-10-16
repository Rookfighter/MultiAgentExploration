#ifndef MAE_PIONEER_2DX_hpp
#define MAE_PIONEER_2DX_hpp

#include "control/ExplorationBot.hpp"

namespace mae
{

	class Pioneer2DX : public ExplorationBot
	{
	public:
		Pioneer2DX(PlayerClient *p_client,
		           Simulation *p_simulation,
		           const std::string &p_name,
		           const int p_motorIndex,
		           const int p_rangerIndex);
		~Pioneer2DX();

		Velocity getMaxVelocity() const;
		Velocity getMinVelocity() const;
	};

}

#endif
