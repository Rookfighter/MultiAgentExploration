#ifndef MAE_COMPASS_ALGORITHM_HPP
#define MAE_COMPASS_ALGORITHM_HPP

#include "algorithm/Algorithm.hpp"
#include "algorithm/AlgorithmConfig.hpp"
#include "algorithm-compass/CompassState.hpp"

namespace mae
{

	class CompassAlgorithm : public Algorithm
	{
	private:
		CompassState *state_;
	public:
		CompassAlgorithm(const AlgorithmConfig &p_config);
		~CompassAlgorithm();
		
		void update();
	};

}

#endif
