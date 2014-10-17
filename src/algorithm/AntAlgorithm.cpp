#include "algorithm/AntAlgorithm.hpp"

namespace mae
{

	AntAlgorithm::AntAlgorithm(ExplorationBot &p_robot,
		             Simulation &p_simulation,
		             MarkerStock &p_stock)
					 :robot_(p_robot), simulation_(p_simulation), stock_(p_stock)
	{
	}

	AntAlgorithm::~AntAlgorithm()
	{
	}


}
