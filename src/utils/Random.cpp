#include <climits>
#include <cstdlib>
#include <ctime>
#include "Random.hpp"

namespace mae
{
	bool Random::initialized_ = false;
	
	Random::Random()
	{
		if(!initialized_) {
			std::srand(static_cast<unsigned> (std::time(NULL)));
			initialized_ = true;
		}
	}

	Random::~Random()
	{
	}
	
	int Random::nextInt()
	{
		return std::rand();
	}
	
	int Random::nextInt(const int p_range)
	{
		return nextInt() % p_range;
	}
	
	float Random::nextFloat()
	{
		return static_cast<float> (std::rand()) / static_cast<float> (RAND_MAX);
	}
	
	bool Random::nextBool()
	{
		return nextInt(2) == 0;
	}


}

