#ifndef MAE_PERFORMANCE_TEST_HPP
#define MAE_PERFORMANCE_TEST_HPP

#include "utils/Math.hpp"
#include "utils/StopWatch.hpp"

namespace mae
{

	class PerformanceTest
	{
	private:
		StopWatch watch_;
		
	public:
		volatile double result;
		
		PerformanceTest();
		~PerformanceTest();
		
		void execute();
	};

}

#endif
