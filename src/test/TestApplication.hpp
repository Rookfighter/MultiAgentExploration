#ifndef MA_TEST_APPLICATION_HPP
#define MA_TEST_APPLICATION_HPP

#include <stdexcept>
#include "control/SimulationLoader.hpp"
#include "test/BasicTest.hpp"
#include "test/HeisenbergTest.hpp"
#include "test/WanderTest.hpp"

namespace mae
{

class TestApplication
	{
	private:
		SimulationLoader loader_;
		BasicTest *basicTest_;
		HeisenbergTest *heisenbergTest_;
		WanderTest *wanderTest_;

	public:
		TestApplication();

		~TestApplication();

		void run();
	};

}

#endif
