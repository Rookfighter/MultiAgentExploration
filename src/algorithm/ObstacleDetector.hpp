#ifndef MAE_OBSTACLE_DETECTOR_HPP
#define MAE_OBSTACLE_DETECTOR_HPP

#include "simulation/ExplorationBot.hpp"

namespace mae
{

	class ObstacleDetector
	{
	private:
		ExplorationBot *robot_;
	public:
		ObstacleDetector(ExplorationBot *p_robot);
		~ObstacleDetector();

		bool check(const double p_beginAngle,
		           const double p_endAngle,
		           const double p_distance) const;

		double getMinDistance(const double p_beginAngle,
		                      const double p_endAngle) const;
	};

}

#endif
