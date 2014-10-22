#ifndef MAE_HEISENBERG_TEST_HPP
#define MAE_HEISENBERG_TEST_HPP

#include "algorithm/HeisenbergObstacleAvoid.hpp"
#include "simulation/ExplorationBot.hpp"

namespace mae
{

	class HeisenbergTest
	{
	private:
		PlayerClient &client_;
		ExplorationBot &bot_;
		HeisenbergObstacleAvoid heisenberg_;
	public:
		HeisenbergTest(PlayerClient &p_client,
		               ExplorationBot &p_bot);
		~HeisenbergTest();

		void execute();

	};

}

#endif
