#include <easylogging++.h>
#include "test/PerformanceTest.hpp"

#define TEST_COUNT 500000
#define SQRT_TEST 0.0001
#define SIN_TEST 0.023825338
#define COS_TEST SIN_TEST
#define ATAN_TEST_Y 0.00001
#define ATAN_TEST_X 0.00001

namespace mae
{

	PerformanceTest::PerformanceTest()
	{
	}

	PerformanceTest::~PerformanceTest()
	{
	}

	void PerformanceTest::execute()
	{
		LOG(INFO) << "std sqrt test";
		LOG(INFO) << "=============";
		watch_.start();
		for(int i = 0; i < TEST_COUNT; ++i)
			result = sqrt(i);
		watch_.stop();
		LOG(INFO) << "Measured: " << watch_.getLastUsec() << "us";
		
		LOG(INFO) << "std sin test";
		LOG(INFO) << "=============";
		watch_.start();
		for(int i = 0; i < TEST_COUNT; ++i)
			result = sin(i);
		watch_.stop();
		LOG(INFO) << "Measured: " << watch_.getLastUsec() << "us";
		
		LOG(INFO) << "std cos test";
		LOG(INFO) << "=============";
		watch_.start();
		for(int i = 0; i < TEST_COUNT; ++i)
			result = cos(i);
		watch_.stop();
		LOG(INFO) << "Measured: " << watch_.getLastUsec() << "us";
		
		LOG(INFO) << "std atan test";
		LOG(INFO) << "=============";
		watch_.start();
		for(int i = 0; i < TEST_COUNT; ++i)
			result = atan2(i, i);
		watch_.stop();
		LOG(INFO) << "Measured: " << watch_.getLastUsec() << "us";
	}
}
