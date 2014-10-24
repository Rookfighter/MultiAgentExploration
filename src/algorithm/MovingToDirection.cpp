#include <easylogging++.h>
#include "algorithm/MovingToDirection.hpp"
#include "algorithm/DroppingMarker.hpp"
#include "common/Math.hpp"

/* determines how much of the max angular
 * velocity is used */
#define TURN_FACTOR 0.8

/* determines how much of marker range should
 * be moved until we place new marker */
#define MOVEMENT_FACTOR 0.9

namespace mae
{

	MovingToDirection::MovingToDirection(const AntStateProperties &p_properties)
		: AntState(p_properties), wander_(*p_properties.robot),
		  lastPose_(p_properties.robot->getMotor().getPose()), movedDistance_(0),
		  turnedAngle_(0)
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
		if(reachedTarget())
			return new DroppingMarker(properties_);
		else if(reachedDirection())
			move();
		else {
			LOG(DEBUG) << "Angle to turn: " << properties_.angleToTurn << " turned angle: " << turnedAngle_;
			turn();
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

	bool MovingToDirection::reachedTarget()
	{
		assert(properties_.currentMarker != NULL);
		return movedDistance_ >= (MOVEMENT_FACTOR * properties_.currentMarker->getRange());
	}
	
	bool MovingToDirection::reachedDirection()
	{
		return fabs(turnedAngle_) >= fabs(properties_.angleToTurn);
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
