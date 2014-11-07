#ifndef MAE_NODE_COUNTING_HPP
#define MAE_NODE_COUNTING_HPP

#include "algorithm-rt/AntAlgorithm.hpp"
#include "algorithm/AlgorithmConfig.hpp"

namespace mae
{
	class NodeCounting : public AntAlgorithm
	{
	public:
		NodeCounting(const AlgorithmConfig &p_config);
		~NodeCounting();

	};

}

#endif
