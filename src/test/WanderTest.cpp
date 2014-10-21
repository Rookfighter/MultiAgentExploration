#include "test/WanderTest.hpp"

namespace mae
{

	WanderTest::WanderTest(PlayerClient &p_client,
	                       ExplorationBot &p_bot)
		:client_(p_client), robot_(p_bot), wander_(robot_)
	{
	}

	WanderTest::~WanderTest()
	{
	}


	void WanderTest::execute()
	{
		while(true) {
			client_.update();
			wander_.step();
		}
	}

}
