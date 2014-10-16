#ifndef MAE_HEISENBERG_OBSTACLE_AVOID_HPP
#define MAE_HEISENBERG_OBSTACLE_AVOID_HPP

#include "control/ExplorationBot.hpp"

namespace mae
{
	
	class HeisenbergObstacleAvoid
	{
	private:
		ExplorationBot &robot_;
		RangerConfig config_;
		std::vector<double> distance_;
		Velocity velocity_;
		double distanceThreshold_;
		
		void updateDistance();
		void avoidObstalce(const int p_sensorID);
		
	public:
		HeisenbergObstacleAvoid(ExplorationBot &p_robot, const double p_distanceThreshold = 1.0);
		~HeisenbergObstacleAvoid();
		
		void execute();
	};

}

#endif
