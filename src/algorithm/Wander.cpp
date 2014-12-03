#include <easylogging++.h>
#include "algorithm/Wander.hpp"
#include "utils/Math.hpp"

#define LEFT_ANGLE_BEGIN (M_PI / 6) // 30°
#define LEFT_ANGLE_END (7 * M_PI / 12) // 105°
#define RIGHT_ANGLE_BEGIN (-7 * M_PI / 12) // -105°
#define RIGHT_ANGLE_END (-M_PI / 6) // -30°
#define FRONT_ANGLE_BEGIN (-M_PI / 6) // -30°
#define FRONT_ANGLE_END (M_PI / 6) // 30

#define NEARBY_SAFETY_DISTANCE 0.2

namespace mae
{
	Wander::Wander(ExplorationBot *p_robot,
	               const double p_frontStopDistance,
                   const double p_avoidDistance)
		:robot_(p_robot),
		 obstacleDetector_(p_robot),
		 frontStopDistance_(p_frontStopDistance),
         avoidDistance_(p_avoidDistance),
		 avoidStep_(0),
		 stopStep_(0)
	{
	}

	Wander::~Wander()
	{

	}

	bool Wander::hasLeftObstacle() const
	{
	    return obstacleDetector_.check(LEFT_ANGLE_BEGIN,
	                                   LEFT_ANGLE_END,
	                                   avoidDistance_);
	}

    bool Wander::hasRightObstacle() const
    {
        return obstacleDetector_.check(RIGHT_ANGLE_BEGIN,
                                       RIGHT_ANGLE_END,
                                       avoidDistance_);
    }
    
    bool Wander::hasfrontStopObstacle() const
    {
        return obstacleDetector_.check(FRONT_ANGLE_BEGIN,
                                       FRONT_ANGLE_END,
                                       frontStopDistance_);
    }

    double Wander::getLeftMinDistance() const
    {
        return obstacleDetector_.getMinDistance(LEFT_ANGLE_BEGIN, LEFT_ANGLE_END);
    }

    double Wander::getRightMinDistance() const
    {
        return obstacleDetector_.getMinDistance(RIGHT_ANGLE_BEGIN, RIGHT_ANGLE_END);
    }

    bool Wander::isAvoidingObstacle() const
    {
        return hasRightObstacle() || hasLeftObstacle() || hasfrontStopObstacle();
    }

	void Wander::update()
	{
		// no valid data, so we can't use sensors
		if(!robot_->getRanger().hasValidData()) {
			robot_->getMotor().stop();
			return;
		}

		if(hasfrontStopObstacle()) {
		    avoidStep_ = 0;
			stopAndAvoidObstacle();
		} else if(hasLeftObstacle() || hasRightObstacle()) {
            stopStep_ = 0;
            avoidNearbyObstacle();
        } else {
            avoidStep_ = 0;
            stopStep_ = 0;
			cruise();
        }

		robot_->getMotor().setVelocity(velocity_);
	}
    
	void Wander::stopAndAvoidObstacle()
	{
		// only change behavior after a certain interval
		// prohibits that robots keeps changing direction every step
		if(stopStep_ % STOP_STEP_REFRESH_INTERVAL == 0) {
			if(getLeftMinDistance() < getRightMinDistance())
				velocity_.angular = robot_->getMotor().getMinVelocity().angular;
			else
				velocity_.angular = robot_->getMotor().getMaxVelocity().angular;

			velocity_.linear = 0;
			stopStep_ = 0;
		}

		stopStep_++;
	}
    
    void Wander::avoidNearbyObstacle()
    {
        int measureCount = 0;
        double leftRightDiff = 0;
        
        // only change behavior after a certain interval
        // prohibits that robots keeps changing direction every step
        if(avoidStep_ % AVOID_STEP_REFRESH_INTERVAL == 0) {
            LOG(DEBUG) << "-- avoiding nearby obstacle";

            if(hasLeftObstacle()) {
                double safeLeftDistance = getLeftMinDistance() - NEARBY_SAFETY_DISTANCE;
                if(safeLeftDistance < 0)
                    safeLeftDistance = 0;
                double safeAvoidDistance = avoidDistance_ - NEARBY_SAFETY_DISTANCE;
                leftRightDiff -= (1.0 - (safeLeftDistance / safeAvoidDistance));
                measureCount++;
            }
            if(hasRightObstacle()) {
                double safeRightDistance = getRightMinDistance() - NEARBY_SAFETY_DISTANCE;
                if(safeRightDistance < 0)
                    safeRightDistance = 0;
                double safeAvoidDistance = avoidDistance_ - NEARBY_SAFETY_DISTANCE;
                leftRightDiff += (1.0 - (safeRightDistance / safeAvoidDistance));
                measureCount++;
            }

            if(measureCount != 0)
                leftRightDiff /= measureCount;
            
            LOG(DEBUG) << "-- leftRightDiff:" << leftRightDiff;

            if(leftRightDiff > 0)
                velocity_.angular = fabs(leftRightDiff) * robot_->getMotor().getMaxVelocity().angular;
            else
                velocity_.angular = fabs(leftRightDiff) * robot_->getMotor().getMinVelocity().angular;

            velocity_.linear = (1.0 - fabs(leftRightDiff)) * robot_->getMotor().getMaxVelocity().linear;

            LOG(DEBUG) << "-- set velocity to " << velocity_.str();

            avoidStep_ = 0;
        }

        avoidStep_++;
    }

	void Wander::cruise()
	{
		velocity_ = robot_->getMotor().getMaxVelocity();
		velocity_.angular = 0;
	}
}
