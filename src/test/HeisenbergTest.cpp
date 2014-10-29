#include "test/HeisenbergTest.hpp"

namespace mae
{

	HeisenbergTest::HeisenbergTest(World &p_world,
	                               ExplorationBot &p_bot)
		:world_(p_world), heisenberg_(p_bot)
	{

	}
	
	HeisenbergTest::~HeisenbergTest()
	{

	}

	void HeisenbergTest::execute()
	{
		while(true) {
			world_.update();
			heisenberg_.execute();
		}
	}


}
