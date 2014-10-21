#include "algorithm/Wander.hpp"
#include "common/Math.hpp"

#define MEASURE_BEGIN_ANGLE (M_PI / 2) // 90°
#define MEASURE_END_ANGLE (-M_PI / 2)  // -90°
#define FRONT_BEGIN_ANGLE (M_PI / 6)   // 30°
#define FRONT_END_ANGLE (-M_PI / 6)    // -30°

#define LINEAR_OBSTRUCT_FACTOR 0.4

namespace mae
{
	Wander::Wander(ExplorationBot &p_robot,
	               const double p_frontStopDistance)
		:robot_(p_robot),
		 frontStopDistance_(p_frontStopDistance)
	{
	}

	Wander::~Wander()
	{

	}

	void Wander::step()
	{
		// no valid data, so we can't use sensors
		if(!robot_.hasValidRangerData()) {
			robot_.setVelocity(Velocity(0,0));
			return;
		}
		
		checkObstacle();

		if(minFrontDistance_ <= frontStopDistance_)
			avoidObstacle();
		else
			cruise();

		robot_.setVelocity(velocity_);
	}

	void Wander::checkObstacle()
	{
		const RangerConfig& config = robot_.getRangerConfig();

		minFrontDistance_ = 1e6;
		minLeftDistance_ = 1e6;
		minRightDistance_ = 1e6;

		for(int i = 0; i < config.sensorCount; ++i) {
			// normalize yaw in [-pi;pi]
			double normalizedYaw = normalizeRadian(config.sensorPose[i].yaw);
			// don't use rangers with yaw bigger than 90°
			if(fabs(normalizedYaw) > MEASURE_BEGIN_ANGLE)
				continue;

			double measuredDistance = robot_.getRangerDistance(i);
			if(normalizedYaw > FRONT_BEGIN_ANGLE) {
				// left side
				minLeftDistance_ = std::min(minLeftDistance_, measuredDistance);
			} else if(normalizedYaw < FRONT_END_ANGLE) {
				// right side
				minRightDistance_ = std::min(minRightDistance_, measuredDistance);
			} else {
				// front side
				minFrontDistance_ = std::min(minFrontDistance_, measuredDistance);
			}
		}
	}

	void Wander::avoidObstacle()
	{
		if(minLeftDistance_ < minRightDistance_)
			velocity_.angular = robot_.getMinVelocity().angular;
		else
			velocity_.angular = robot_.getMaxVelocity().angular;
			
		velocity_.linear = 0;
	}
	
	void Wander::cruise()
	{
		velocity_ = robot_.getMaxVelocity();
		velocity_.angular = 0;
	}


}
