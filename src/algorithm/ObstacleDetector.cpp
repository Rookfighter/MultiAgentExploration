#include "algorithm/ObstacleDetector.hpp"
#include "utils/Math.hpp"

namespace mae
{

	ObstacleDetector::ObstacleDetector(ExplorationBot *p_robot)
		: robot_(p_robot)
	{
	}

	ObstacleDetector::~ObstacleDetector()
	{
	}

	bool ObstacleDetector::check(const double p_beginAngle,
	                             const double p_endAngle,
	                             const double p_distance) const
	{
		return getMinDistance(p_beginAngle,p_endAngle) < p_distance;
	}

	bool ObstacleDetector::check(const int p_sensor, const double p_distance) const
	{
	    return robot_->getRanger().getDistance(p_sensor) <= p_distance;
	}

	double ObstacleDetector::getMinDistance(const double p_beginAngle,
	                                        const double p_endAngle) const
	{
		double minDistance = 1e6;

		const RangerProperties& rangerProperties = robot_->getRanger().getProperties();
		for(unsigned int i = 0; i < rangerProperties.getMeasurementCount(); ++i) {

			// check if sensor is in the angles we check
			if(!angleIsBetween(rangerProperties.getMeasurementOrigins()[i].yaw, p_beginAngle, p_endAngle))
				continue;

			double distance = robot_->getRanger().getDistance(i);
			if(distance < minDistance)
				minDistance = distance;
		}

		return minDistance;
	}
}
