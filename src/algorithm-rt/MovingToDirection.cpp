#include <easylogging++.h>
#include "algorithm-rt/MovingToDirection.hpp"
#include "algorithm-rt/DroppingMarker.hpp"
#include "utils/Math.hpp"

/* determines how much of the max angular
 * velocity is used */
#define TURN_FACTOR 0.8

/* determines how much of marker range should
 * be moved until we place new marker */
#define MOVEMENT_FACTOR 0.8

/* precision of turn angle check */
#define TURN_EPS ((M_PI / 180) * 5) // precision 5°

#define FRONT_ANGLE_BEGIN (-M_PI / 6)
#define FRONT_ANGLE_END (M_PI / 6)

namespace mae
{

	MovingToDirection::MovingToDirection(const AntStateProperties &p_properties)
		: AntState(p_properties), wander_(p_properties.robot, p_properties.obstacleAvoidDistance),
		  obstacleDetector_(p_properties.robot),
		  lastPose_(p_properties.robot->getMotor().getPose()), movedDistance_(0),
		  reachedDirection_(false)
	{
		LOG(DEBUG) << "Changed to MovingToDirection state";
		remainingToTurn_ = normalizeRadian(properties_.angleToTurn);
	}

	MovingToDirection::~MovingToDirection()
	{
	}

	AntState* MovingToDirection::update()
	{
		updateGeometry();
		if(!reachedDirection_)
			reachedDirection_ = reachedDirection();

		if(!reachedDirection_) {
			turn();
		} else {
			if(movedEnough() || hasFrontObstacle()) {
				properties_.robot->getMotor().stop();
				return new DroppingMarker(properties_);
			} else
				move();
		}

		return NULL;
	}

	void MovingToDirection::updateGeometry()
	{
		// get moved distance since last call
		Pose currentPose = properties_.robot->getMotor().getPose();
		movedDistance_ += (currentPose.position - lastPose_.position).length();
		remainingToTurn_ -= normalizeRadian(currentPose.yaw - lastPose_.yaw);
		lastPose_ = currentPose;
	}

	bool MovingToDirection::reachedDirection()
	{
		return sameDouble(remainingToTurn_,
		                  0,
		                  TURN_EPS);
	}

	bool MovingToDirection::movedEnough()
	{
		return movedDistance_ >= MOVEMENT_FACTOR * properties_.robot->getMarkerSensor().getMaxRange();
	}

	bool MovingToDirection::hasFrontObstacle()
	{
		return obstacleDetector_.check(FRONT_ANGLE_BEGIN,
		                               FRONT_ANGLE_END,
		                               properties_.obstacleAvoidDistance);
	}

	void MovingToDirection::move()
	{
		wander_.step();
	}

	void MovingToDirection::turn()
	{
		double angularVelocity;
		if(remainingToTurn_ < 0)
			angularVelocity = properties_.robot->getMotor().getMinVelocity().angular * TURN_FACTOR;
		else
			angularVelocity = properties_.robot->getMotor().getMaxVelocity().angular * TURN_FACTOR;

		properties_.robot->getMotor().setVelocity(Velocity(0, angularVelocity));
	}

}
