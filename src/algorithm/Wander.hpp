#ifndef MAE_WANDER_HPP
#define MAE_WANDER_HPP

#include "control/ExplorationBot.hpp"

namespace mae
{

	class Wander
	{
	private:
		ExplorationBot &robot_;
		Velocity velocity_;
		
		double minLeftDistance_;
		double minRightDistance_;
		double frontThreshold;
		
		void checkObstacle();
		void avoidObstacle();
		void cruise();
	public:
		Wander(ExplorationBot &p_robot, const double p_frontThreshold = 1.0);
		~Wander();
		
		void execute();
	};

}

#endif
