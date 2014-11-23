#include "algorithm/Wander.hpp"
#include "utils/Math.hpp"

#define LEFT_ANGLE_BEGIN (M_PI / 6) // 30°
#define LEFT_ANGLE_END (M_PI / 2) // 90°
#define RIGHT_ANGLE_BEGIN (-M_PI / 2) // -90°
#define RIGHT_ANGLE_END (-M_PI / 6) // -30°
#define FRONT_ANGLE_BEGIN (-M_PI / 6) // -30°
#define FRONT_ANGLE_END (M_PI / 6) // 30

#define LINEAR_OBSTRUCT_FACTOR 0.4

namespace mae
{
	Wander::Wander(ExplorationBot *p_robot,
	               const double p_frontStopDistance,
                   const double p_avoidDistance)
		:robot_(p_robot),
		 obstacleDetector_(p_robot),
		 frontStopDistance_(p_frontStopDistance),
         avoidDistance_(p_avoidDistance),
		 stopRobot_(false),
		 avoidStep_(0)
	{
	}

	Wander::~Wander()
	{

	}

	void Wander::update()
	{
		// no valid data, so we can't use sensors
		if(!robot_->getRanger().hasValidData()) {
			robot_->getMotor().stop();
			return;
		}

		checkObstacle();

		if(stopRobot_)
			stopAndAvoidObstacle();
        else if(hasNearbyObstacle())
            avoidNearbyObstacle();
		else
			cruise();

		robot_->getMotor().setVelocity(velocity_);
	}

	void Wander::checkObstacle()
	{
		minFrontDistance_ = obstacleDetector_.getMinDistance(FRONT_ANGLE_BEGIN, FRONT_ANGLE_END);
		minLeftDistance_ = obstacleDetector_.getMinDistance(LEFT_ANGLE_BEGIN, LEFT_ANGLE_END);
		minRightDistance_ = obstacleDetector_.getMinDistance(RIGHT_ANGLE_BEGIN, RIGHT_ANGLE_END);

		bool tmpStopRobot = minFrontDistance_ <= frontStopDistance_;
		if(!stopRobot_ && tmpStopRobot)
			beginAvoiding();
		if(stopRobot_ && !tmpStopRobot)
			endAvoiding();
		stopRobot_ = tmpStopRobot;
	}

	void Wander::stopAndAvoidObstacle()
	{
		// only change behavior after a certain interval
		// prohibits that robots keeps changing direction every step
		if(avoidStep_ % STEP_REFRESH_INTERVAL == 0) {
			if(minLeftDistance_ < minRightDistance_)
				velocity_.angular = robot_->getMotor().getMinVelocity().angular;
			else
				velocity_.angular = robot_->getMotor().getMaxVelocity().angular;

			velocity_.linear = 0;
			avoidStep_ = 0;
		}

		avoidStep_++;
	}
    
    bool Wander::hasNearbyObstacle()
    {
        return minLeftDistance_ <= avoidDistance_ || minRightDistance_ <= avoidDistance_;
    }
    
    void Wander::avoidNearbyObstacle()
    {
        int measureCount = 0;
        double leftRightDiff = 0;
        
        if(minLeftDistance_ <= avoidDistance_) {
            leftRightDiff += (1.0 - (minLeftDistance_ / avoidDistance_));
            measureCount++;
        }
        if(minRightDistance_ <= avoidDistance_) {
            leftRightDiff -= (1.0 - (minRightDistance_ / avoidDistance_));
            measureCount++;
        }
        leftRightDiff /= measureCount;
        
        if(leftRightDiff > 0)
            velocity_.angular = abs(leftRightDiff) * robot_->getMotor().getMaxVelocity().angular;
        else
            velocity_.angular = abs(leftRightDiff) * robot_->getMotor().getMinVelocity().angular;
            
        velocity_.linear = (1.0 - abs(leftRightDiff)) * robot_->getMotor().getMaxVelocity().linear;
    }

	void Wander::cruise()
	{
		velocity_ = robot_->getMotor().getMaxVelocity();
		velocity_.angular = 0;
	}

	void Wander::beginAvoiding()
	{
		if(avoidBeginCallback_)
			avoidBeginCallback_();
	}

	void Wander::endAvoiding()
	{
		avoidStep_ = 0;
		if(avoidEndCallback_)
			avoidEndCallback_();
	}

	void Wander::onAvoidBegin(std::function<void(void)> p_callback)
	{
		avoidBeginCallback_ = p_callback;
	}

	void Wander::onAvoidEnd(std::function<void(void)> p_callback)
	{
		avoidEndCallback_ = p_callback;
	}


}
