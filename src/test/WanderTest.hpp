#ifndef MAE_WANDER_TEST_HPP
#define MAE_WANDER_TEST_HPP

#include "simulation/World.hpp"
#include "algorithm/Wander.hpp"

namespace mae
{

	class WanderTest
	{
	private:
		World &world_;
		Wander wander_;
	public:
		WanderTest(World &p_world,
		           ExplorationBot &p_bot);
		~WanderTest();

		void execute();
	};

}

#endif
