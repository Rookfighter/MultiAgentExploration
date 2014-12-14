#include <climits>
#include <cstdlib>
#include <ctime>
#include "Random.hpp"

namespace mae
{
	bool Random::initialized_ = false;
	
	void Random::init()
	{
	    std::srand(static_cast<unsigned> (std::time(NULL)));
	    initialized_ = true;
	}

	int Random::nextInt()
	{
	    if(!initialized_)
	        init();
		return std::rand();
	}
	
	int Random::nextInt(const int p_range)
	{
		return nextInt() % p_range;
	}
	
	float Random::nextFloat()
	{
		return static_cast<float>(nextInt()) / static_cast<float> (RAND_MAX);
	}
	
	bool Random::nextBool()
	{
		return nextInt(2) == 0;
	}


}

