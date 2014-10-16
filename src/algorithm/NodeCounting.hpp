#ifndef MAE_NODE_COUNTING_HPP
#define MAE_NODE_COUNTING_HPP

namespace mae
{
	class NodeCountingState
	{
		NodeCountingState* update();
	};

	class NodeCounting
	{
	private:
		ExplorationBot &robot_;
		Simulation &simulation_;
		MarkerStock &stock_;

	public:
		NodeCounting(ExplorationBot &p_robot,
		             Simulation &p_simulation,
		             MarkerStock &p_stock);
		~NodeCounting();

		void step();
	};

}

#endif
