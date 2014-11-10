#include "test/WanderTest.hpp"

namespace mae
{

	WanderTest::WanderTest(Simulation *p_world,
	                       ExplorationBot *p_bot)
		:world_(p_world), wander_(p_bot)
	{
	}

	WanderTest::~WanderTest()
	{
	}


	void WanderTest::execute()
	{
		while(true) {
			world_->update();
			wander_.step();
		}
	}

}
