#ifndef MAE_NODE_COUNTING_HPP
#define MAE_NODE_COUNTING_HPP

#include "algorithm/AntAlgorithm.hpp"

namespace mae
{
	class NodeCounting : public AntAlgorithm
	{
	public:
		NodeCounting(ExplorationBot *p_robot,
		             Simulation *p_simulation,
		             MarkerStock *p_stock);
		~NodeCounting();

	};

}

#endif
