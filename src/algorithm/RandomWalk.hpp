#ifndef MAE_RANDOM_WALK_HPP
#define MAE_RANDOM_WALK_HPP

#include "algorithm/Algorithm.hpp"
#include "algorithm/AlgorithmConfig.hpp"
#include "algorithm/MovementController.hpp"
#include "utils/Random.hpp"

namespace mae
{

	class RandomWalk : public Algorithm
	{
	private:
		MovementController robotController_;
		Random random_;
		double wanderDistance_;
		
		void chooseRandomDirection();
	public:
		RandomWalk(const AlgorithmConfig &p_config);
		~RandomWalk();
		
		void update();
	};

}

#endif
