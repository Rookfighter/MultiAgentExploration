#ifndef MAE_LRTAStar_HPP
#define MAE_LRTAStar_HPP

#include "algorithm-rt/AntAlgorithm.hpp"
#include "algorithm/AlgorithmConfig.hpp"

namespace mae
{

	class LRTAStar : public AntAlgorithm
	{
	private:
		void updateValue(Marker* p_current, Marker* p_next);
		double calcValue(Marker* p_current, Marker* p_next);
	public:
		LRTAStar(const AlgorithmConfig &p_config);
		~LRTAStar();

	};

}

#endif
