#include <easylogging++.h>
#include "algorithm/HeisenbergObstacleAvoid.hpp"
#include "common/Math.hpp"

#define FRONT_ANGLE degreeToRadian(10)
#define ZERO_EPSILON 0.001

namespace mae
{

	HeisenbergObstacleAvoid::HeisenbergObstacleAvoid(ExplorationBot &p_robot,  const double p_distanceThreshold)
		: robot_(p_robot), config_(robot_.getRanger().getProperties()),
		  distance_(config_.getMeasurementCount()), distanceThreshold_(p_distanceThreshold)
	{
	}

	HeisenbergObstacleAvoid::~HeisenbergObstacleAvoid()
	{
	}

	void HeisenbergObstacleAvoid::execute()
	{
		if(robot_.getRanger().hasValidData()) {
			updateDistance();

			velocity_.set(1.0, 0);

			for(int i = 0; i < config_.getMeasurementCount(); ++i) {
				if(distance_[i] <= distanceThreshold_)
					avoidObstalce(i);
			}

			velocity_.linear *= robot_.getMotor().getMaxVelocity().linear;
			velocity_.angular *= robot_.getMotor().getMaxVelocity().angular;
			robot_.getMotor().setVelocity(velocity_);
		} else {
			LOG(DEBUG) << "Heisenberg: No valid ranger data";
		}
	}

	void HeisenbergObstacleAvoid::updateDistance()
	{
		for(int i = 0; i < config_.getMeasurementCount(); ++i) {
			distance_[i] = robot_.getRanger().getDistance(i);
		}
	}

	void HeisenbergObstacleAvoid::avoidObstalce(const int p_sensorID)
	{
		// normalize yaw between [-pi;pi]
		double normalizedYaw = normalizeRadian(config_.getMeasurementOrigins()[p_sensorID].yaw);
		// if yaw is close to front then direction gain is high
		double directionGain = 1 - fabs(normalizedYaw / (2 * M_PI));
		double toChange = velocity_.linear * directionGain *
		                  ((distanceThreshold_ - distance_[p_sensorID]) / distanceThreshold_);
		if(normalizedYaw < 0)
			velocity_.angular += toChange;
		else if(normalizedYaw > 0)
			velocity_.angular -= toChange;
		else {
			if(sameDouble(velocity_.angular, 0, ZERO_EPSILON)) {
				// angular is too small
				velocity_.angular = 0;
			} else {
				// increase by current sign
				velocity_.angular += (fabs(velocity_.angular) / velocity_.angular) * toChange;
			}
		}

		velocity_.linear = 1 - velocity_.angular;
	}
}