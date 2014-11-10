#ifndef MAE_NODE_COUNTING_HPP
#define MAE_NODE_COUNTING_HPP

#include "algorithm-rt/AntAlgorithm.hpp"
#include "algorithm/AlgorithmConfig.hpp"

namespace mae
{
	class NodeCounting : public AntAlgorithm
	{
	private:
		void updateValue(Marker* p_current, Marker* p_next);
		double calcValue(Marker* p_current, Marker* p_next);
	public:
		NodeCounting(const AlgorithmConfig &p_config);
		~NodeCounting();

	};

}

#endif
