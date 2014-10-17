#ifndef MAE_WANDER_TEST_HPP
#define MAE_WANDER_TEST_HPP

#include "control/ExplorationBot.hpp"
#include "algorithm/Wander.hpp"

namespace mae
{

	class WanderTest
	{
	private:
		PlayerClient &client_;
		ExplorationBot &robot_;
		Wander wander_;
	public:
		WanderTest(PlayerClient &p_client,
		           ExplorationBot &p_bot);
		~WanderTest();

		void step();
	};

}

#endif
