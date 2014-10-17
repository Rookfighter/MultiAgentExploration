#ifndef MAE_ANT_ALGORITHM_HPP
#define MAE_ANT_ALGORITHM_HPP

#include "control/ExplorationBot.hpp"
#include "control/MarkerStock.hpp"

namespace mae
{
	
	class AntAlgorithm
	{
	protected:
		ExplorationBot &robot_;
		Simulation &simulation_;
		MarkerStock &stock_;

	public:
		AntAlgorithm(ExplorationBot &p_robot,
		             Simulation &p_simulation,
		             MarkerStock &p_stock);
		virtual ~AntAlgorithm();

	};

}

#endif
