#ifndef MAE_ANT_STATES_HPP
#define MAE_ANT_STATES_HPP

#include "algorithm/AntUpdateRules.hpp"

namespace mae
{

	class AntState
	{
	protected:
		ExplorationBot &robot_;
		Simulation &simulation_;
		MarkerStock &stock_;
		AntUpdateRule &updateRule_;

	public:
		AntState(ExplorationBot &p_robot,
		         Simulation &p_simulation,
		         MarkerStock &p_stock,
		         AntUpdateRule &p_updateRule)
			:robot_(p_robot), simulation_(p_simulation),
			 stock_(p_stock), updateRule_(p_updateRule) { }

		virtual ~AntState() { }

		virtual AntState* update() = 0;
	};

	class AntStateOrientating : public AntState
	{
	public:
		AntStateOrientating(ExplorationBot &p_robot,
		                    Simulation &p_simulation,
		                    MarkerStock &p_stock,
		                    AntUpdateRule &p_updateRule)
			:AntState(p_robot, p_simulation, p_stock, p_updateRule) { }

		~AntStateOrientating() { }

		AntState* update();
	};

	class AntStateWandering : public AntState
	{
	private:
		double distance_;
		double maxDistance_;
	public:
		AntStateWandering(ExplorationBot &p_robot,
		                    Simulation &p_simulation,
		                    MarkerStock &p_stock,
		                    AntUpdateRule &p_updateRule);
			

		~AntStateWandering() { }

		AntState* update();
	};

	class AntStateDropping : public AntState
	{
	public:
		AntStateDropping(ExplorationBot &p_robot,
		                    Simulation &p_simulation,
		                    MarkerStock &p_stock,
		                    AntUpdateRule &p_updateRule)
			:AntState(p_robot, p_simulation, p_stock, p_updateRule) { }

		~AntStateDropping() { }

		AntState* update();
	};

	class AntStateUpdating : public AntState
	{
	public:
		AntStateUpdating(ExplorationBot &p_robot,
		                    Simulation &p_simulation,
		                    MarkerStock &p_stock,
		                    AntUpdateRule &p_updateRule)
			:AntState(p_robot, p_simulation, p_stock, p_updateRule) { }

		~AntStateUpdating() { }

		AntState* update();
	};

}

#endif
