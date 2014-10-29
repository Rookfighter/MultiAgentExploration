#ifndef MAE_HEISENBERG_TEST_HPP
#define MAE_HEISENBERG_TEST_HPP

#include "algorithm/HeisenbergObstacleAvoid.hpp"
#include "simulation/World.hpp"

namespace mae
{

	class HeisenbergTest
	{
	private:
		World &world_;
		HeisenbergObstacleAvoid heisenberg_;
	public:
		HeisenbergTest(World &p_world,
		               ExplorationBot &p_bot);
		~HeisenbergTest();

		void execute();

	};

}

#endif
