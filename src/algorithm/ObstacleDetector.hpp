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
		bool check(const int p_index, const double p_distance) const;
        bool check(const std::vector<int> &p_sensorIndizes, const double p_distance) const;
        bool checkFront(const double p_distance) const;
        bool checkBack(const double p_distance) const;

		double getMinDistance(const double p_beginAngle,
		                      const double p_endAngle) const;
		double getMinDistance(const std::vector<int> &p_sensorIndizes) const;
		double getFrontMinDistance() const;
		double getBackMinDistance() const;
	};

}

#endif
