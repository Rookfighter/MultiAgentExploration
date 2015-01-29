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

	bool ObstacleDetector::check(const int p_index, const double p_distance) const
    {
        return robot_->getRanger().getDistance(p_index) <= p_distance;
    }

    bool ObstacleDetector::check(const std::vector<int> &p_sensorIndizes, const double p_distance) const
    {
        assert(!p_sensorIndizes.empty());
        for(int idx : p_sensorIndizes) {
            if(check(idx, p_distance))
                return true;
        }

        return false;
    }

    bool ObstacleDetector::checkFront(const double p_distance) const
    {
        bool leftObstacle = robot_->getRanger().getDistance(Ranger::SENSOR_P10) <= p_distance;
        bool rightObstacle = robot_->getRanger().getDistance(Ranger::SENSOR_N10) <= p_distance;

        return leftObstacle || rightObstacle;
    }

    bool ObstacleDetector::checkBack(const double p_distance) const
    {
        bool leftObstacle = robot_->getRanger().getDistance(Ranger::SENSOR_P170) <= p_distance;
        bool rightObstacle = robot_->getRanger().getDistance(Ranger::SENSOR_N170) <= p_distance;

        return leftObstacle || rightObstacle;
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


	double ObstacleDetector::getMinDistance(const std::vector<int> &p_sensorIndizes) const
	{
	    assert(!p_sensorIndizes.empty());
	    double minDistance = 1e6;

	    for(int idx : p_sensorIndizes) {
	        double distance = robot_->getRanger().getDistance(idx);
	        if(distance < minDistance)
	            minDistance = distance;
	    }

	    return minDistance;
	}

	double ObstacleDetector::getFrontMinDistance() const
	{
	    double leftDist = robot_->getRanger().getDistance(Ranger::SENSOR_P10);
	    double rightDist = robot_->getRanger().getDistance(Ranger::SENSOR_N10);
	    return leftDist < rightDist ? leftDist : rightDist;
	}

	double ObstacleDetector::getBackMinDistance() const
	{
	    double leftDist = robot_->getRanger().getDistance(Ranger::SENSOR_P170);
        double rightDist = robot_->getRanger().getDistance(Ranger::SENSOR_N170);
        return leftDist < rightDist ? leftDist : rightDist;
	}
}
