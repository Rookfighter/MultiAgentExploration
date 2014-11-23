#include "test/WanderTest.hpp"

namespace mae
{

	WanderTest::WanderTest(Simulation *p_world,
	                       ExplorationBot *p_bot)
		:world_(p_world), wander_(p_bot, 1.0, 2.0)
	{
	}

	WanderTest::~WanderTest()
	{
	}


	void WanderTest::execute()
	{
		while(true) {
			wander_.update();
		}
	}

}
