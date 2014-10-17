#ifndef MAE_ANT_UPDATE_RULES_HPP
#define MAE_ANT_UPDATE_RULES_HPP

#include "control/ExplorationBot.hpp"
#include "control/MarkerStock.hpp"

namespace mae
{

	class AntUpdateRule
	{
	protected:
		ExplorationBot &robot_;
		Simulation &simulation_;
		MarkerStock &stock_;
	public:
		AntUpdateRule(ExplorationBot &p_robot,
		              Simulation &p_simulation,
		              MarkerStock &p_stock);
		virtual ~AntUpdateRule();

		virtual void update() = 0;

	};

}

#endif
