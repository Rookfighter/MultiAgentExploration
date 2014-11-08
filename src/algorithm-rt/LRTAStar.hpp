#ifndef MAE_LRTAStar_HPP
#define MAE_LRTAStar_HPP

#include "algorithm-rt/AntAlgorithm.hpp"
#include "algorithm/AlgorithmConfig.hpp"

namespace mae
{

	class LRTAStar : public AntAlgorithm
	{
	public:
		LRTAStar(const AlgorithmConfig &p_config);
		~LRTAStar();

	};

}

#endif
