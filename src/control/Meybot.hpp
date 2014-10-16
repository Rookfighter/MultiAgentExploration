#ifndef MAE_MEYBOT_HPP
#define MAE_MEYBOT_HPP

#include "control/ExplorationBot.hpp"

namespace mae
{

	class Meybot : public ExplorationBot
	{
	public:
		const static int FRONT = 0;
		const static int LEFT = 1;
		const static int RIGHT = 2;

		Meybot(PlayerClient *p_client,
		       Simulation *p_simulation,
		       const std::string &p_name,
		       const int p_motorIndex,
		       const int p_rangerIndex);
		~Meybot();

		Velocity getMaxVelocity() const;
		Velocity getMinVelocity() const;
	};

}

#endif
