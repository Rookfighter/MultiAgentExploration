#ifndef MAE_WAGNER_HPP
#define MAE_WAGNER_HPP

#include "algorithm-rt/AntAlgorithm.hpp"
#include "algorithm/AlgorithmConfig.hpp"

namespace mae
{

	class Wagner : public AntAlgorithm
	{
	private:
		void updateValue(Marker* p_current, Marker* p_next);
		double calcValue(Marker* p_current, Marker* p_next);
	public:
		Wagner(const AlgorithmConfig &p_config);
		~Wagner();

	};

}

#endif
