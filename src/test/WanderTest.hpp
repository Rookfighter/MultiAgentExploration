#ifndef MAE_WANDER_TEST_HPP
#define MAE_WANDER_TEST_HPP

#include "simulation/Simulation.hpp"
#include "algorithm/Wander.hpp"

namespace mae
{

	class WanderTest
	{
	private:
		Simulation &world_;
		Wander wander_;
	public:
		WanderTest(Simulation &p_world,
		           ExplorationBot &p_bot);
		~WanderTest();

		void execute();
	};

}

#endif
