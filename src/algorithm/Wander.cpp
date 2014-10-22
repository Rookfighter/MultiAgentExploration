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
		if(!robot_.getRanger().hasValidData()) {
			robot_.getMotor().setVelocity(Velocity(0,0));
			return;
		}
		
		checkObstacle();

		if(minFrontDistance_ <= frontStopDistance_)
			avoidObstacle();
		else
			cruise();

		robot_.getMotor().setVelocity(velocity_);
	}

	void Wander::checkObstacle()
	{
		const RangerProperties& properties = robot_.getRanger().getProperties();

		minFrontDistance_ = 1e6;
		minLeftDistance_ = 1e6;
		minRightDistance_ = 1e6;

		for(int i = 0; i < properties.getMeasurementCount(); ++i) {
			// normalize yaw in [-pi;pi]
			double normalizedYaw = normalizeRadian(properties.getMeasurementOrigins()[i].yaw);
			// don't use rangers with yaw bigger than 90°
			if(fabs(normalizedYaw) > MEASURE_BEGIN_ANGLE)
				continue;

			double measuredDistance = robot_.getRanger().getDistance(i);
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
			velocity_.angular = robot_.getMotor().getMinVelocity().angular;
		else
			velocity_.angular = robot_.getMotor().getMaxVelocity().angular;
			
		velocity_.linear = 0;
	}
	
	void Wander::cruise()
	{
		velocity_ = robot_.getMotor().getMaxVelocity();
		velocity_.angular = 0;
	}


}
