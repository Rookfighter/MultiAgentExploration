#ifndef MA_TEST_APPLICATION_HPP
#define MA_TEST_APPLICATION_HPP

#include <stdexcept>
#include "test/BasicTest.hpp"
#include "test/WanderTest.hpp"
#include "test/NodeCountingTest.hpp"
#include "test/PerformanceTest.hpp"
#include "simulation/World.hpp"

namespace mae
{

class TestApplication
	{
	private:
		World *world_;
		BasicTest *basicTest_;
		WanderTest *wanderTest_;
		NodeCountingTest *ncTest_;
		PerformanceTest *performanceTest_;

	public:
		TestApplication();

		~TestApplication();

		void run();
	};

}

#endif
