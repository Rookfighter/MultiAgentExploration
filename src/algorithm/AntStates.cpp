#include "algorithm/AntStates.hpp"

namespace mae
{

	AntState* AntStateOrientating::update()
	{

	}

	AntStateWandering::AntStateWandering(ExplorationBot &p_robot,
	                                     Simulation &p_simulation,
	                                     MarkerStock &p_stock,
	                                     AntUpdateRule &p_updateRule)
		:AntState(p_robot, p_simulation, p_stock, p_updateRule),
		 distance_(0), maxDistance_(2.0)
	{
		std::vector<Marker*> all = p_stock.getAll();
		if(!all.empty())
			maxDistance = 1.1 * 
	}

	AntState* AntStateWandering::update()
	{

	}

	AntState* AntStateDropping::update()
	{

	}

	AntState* AntStateUpdating::update()
	{

	}


}
