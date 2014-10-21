#include <easylogging++.h>
#include "algorithm/MovingToDirection.hpp"
#include "algorithm/DroppingMarker.hpp"

/* determines how much of the max angular
 * velocity is used */
#define TURN_FACTOR 0.8

/* determines how much fo marker range should
 * be moved until attempt to reach marker is
 * cancelled */
#define MOVEMENT_FACTOR 0.9

namespace mae
{

	MovingToDirection::MovingToDirection(const AntStateProperties &p_properties)
		: AntState(p_properties), wander_(*p_properties.robot),
		  lastPose_(p_properties.robot->getPose()), movedDistance_(0),
		  turnedAngle_(0)
	{
		LOG(DEBUG) << "New MovingToDirection state.";
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
		else
			turn();
			
		return NULL;
	}
	
	void MovingToDirection::updateGeometry()
	{
		// get moved distance since last call
		Pose currentPose = properties_.robot->getPose();
		movedDistance_ += (currentPose.position - lastPose_.position).length();
		turnedAngle_ += currentPose.yaw - lastPose_.yaw;
		lastPose_ = currentPose;
	}

	bool MovingToDirection::reachedTarget()
	{
		assert(properties_.currentMarker != NULL);
		return movedDistance_ >= (MOVEMENT_FACTOR * properties_.currentMarker->range);
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
			angularVelocity = properties_.robot->getMinVelocity().angular * TURN_FACTOR;
		else
			angularVelocity = properties_.robot->getMaxVelocity().angular * TURN_FACTOR;
			
		properties_.robot->setVelocity(Velocity(0, angularVelocity));
	}

}
