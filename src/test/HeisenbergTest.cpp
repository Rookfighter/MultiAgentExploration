#include "test/HeisenbergTest.hpp"

namespace mae
{

	HeisenbergTest::HeisenbergTest(PlayerClient &p_client,
	                               ExplorationBot &p_bot)
		:client_(p_client), bot_(p_bot), heisenberg_(bot_)
	{

	}
	
	HeisenbergTest::~HeisenbergTest()
	{

	}

	void HeisenbergTest::step()
	{
		while(true) {
			client_.update();
			heisenberg_.execute();
		}
	}


}
