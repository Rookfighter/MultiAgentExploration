#include <easylogging++.h>
#include "algorithm/MovingToDirection.hpp"
#include "algorithm/DroppingMarker.hpp"
#include "common/Math.hpp"

/* determines how much of the max angular
 * velocity is used */
#define TURN_FACTOR 0.8

/* determines how much of marker range should
 * be moved until we place new marker */
#define MOVEMENT_FACTOR 0.8

/* precision of turn angle check */
#define TURN_EPS ((M_PI / 180) * 5) // precision 5°

namespace mae
{

	MovingToDirection::MovingToDirection(const AntStateProperties &p_properties)
		: AntState(p_properties), wander_(*p_properties.robot),
		  lastPose_(p_properties.robot->getMotor().getPose()), movedDistance_(0),
		  turnedAngle_(0), reachedDirection_(false)
	{
		LOG(DEBUG) << "New MovingToDirection state.";
		properties_.angleToTurn = normalizeRadian(properties_.angleToTurn);
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
			if(movedEnough())
				return new DroppingMarker(properties_);
			else
				move();
		}

		return NULL;
	}

	void MovingToDirection::updateGeometry()
	{
		// get moved distance since last call
		Pose currentPose = properties_.robot->getMotor().getPose();
		movedDistance_ += (currentPose.position - lastPose_.position).length();
		turnedAngle_ += normalizeRadian(currentPose.yaw - lastPose_.yaw);
		lastPose_ = currentPose;
	}
	
	bool MovingToDirection::reachedDirection()
	{
		return sameDouble(fabs(turnedAngle_),
		                  fabs(properties_.angleToTurn),
		                  TURN_EPS);
	}
	
	bool MovingToDirection::movedEnough()
	{
		assert(properties_.currentMarker != NULL);
		return movedDistance_ >= MOVEMENT_FACTOR * properties_.currentMarker->getRange();
	}

	void MovingToDirection::move()
	{
		wander_.step();
	}

	void MovingToDirection::turn()
	{
		double angularVelocity;
		if(properties_.angleToTurn < 0)
			angularVelocity = properties_.robot->getMotor().getMinVelocity().angular * TURN_FACTOR;
		else
			angularVelocity = properties_.robot->getMotor().getMaxVelocity().angular * TURN_FACTOR;

		properties_.robot->getMotor().setVelocity(Velocity(0, angularVelocity));
	}

}