#include <easylogging++.h>
#include "algorithm/MovingToMarker.hpp"
#include "algorithm/SelectingTarget.hpp"
#include "algorithm/DroppingMarker.hpp"
#include "common/Math.hpp"

/* avoid moving in a direction that is farer
 * away than this (+/-). */
#define ANGLE_EPS (M_PI / 36) // 5°

/* determines how much of the max angular
 * velocity is used */
#define TURN_FACTOR 0.8

/* determines how much of marker range is
 * enough to be close enough*/
#define DISTANCE_EPS 0.05

/* determines how much fo marker range should
 * be moved until attempt to reach marker is
 * cancelled */
#define MOVEMENT_FACTOR 1.1

namespace mae
{

	MovingToMarker::MovingToMarker(const AntStateProperties &p_properties)
		:AntState(p_properties), wander_(*p_properties.robot),
		 lastPose_(p_properties.robot->getMotor().getPose()), movedDistance_(0)
	{
		LOG(DEBUG) << "New MovingToMarker state.";
	}

	MovingToMarker::~MovingToMarker()
	{
	}

	AntState* MovingToMarker::update()
	{
		updateGeometry();

		if(reachedTarget()) {
			// reached the marker, now select next target
			properties_.currentMarker = properties_.nextMarker;
			properties_.robot->getMotor().stop();
			return new SelectingTarget(properties_);
		}
		if(movedEnough()) {
			// moved far, but did not reach marker
			properties_.robot->getMotor().stop();
			return new DroppingMarker(properties_);
		}

		// no target reached, we still have to move
		if(!reachedDirection())
			turnToMarker();
		else
			moveTowardsMarker();

		return NULL;
	}

	void MovingToMarker::updateGeometry()
	{
		// get moved distance since last call
		Pose currentPose = properties_.robot->getMotor().getPose();
		movedDistance_ += (currentPose.position - lastPose_.position).length();
		lastPose_ = currentPose;

		// get angle to target Marker
		angleToTarget_ = properties_.robot->getMarkerSensor().getAngleTo(properties_.nextMarker);
	}

	bool MovingToMarker::reachedDirection()
	{
		return sameDouble(angleToTarget_,
		                  0,
		                  ANGLE_EPS);
	}

	bool MovingToMarker::reachedTarget()
	{
		return sameDouble(properties_.robot->getMarkerSensor().getDistanceTo(properties_.nextMarker).length(),
		                  properties_.nextMarker->getRange(),
		                  DISTANCE_EPS);
	}

	bool MovingToMarker::movedEnough()
	{
		return movedDistance_ >= MOVEMENT_FACTOR * properties_.nextMarker->getRange();
	}

	void MovingToMarker::turnToMarker()
	{
		double angularVelocity;
		if(angleToTarget_ < 0)
			angularVelocity = TURN_FACTOR * properties_.robot->getMotor().getMinVelocity().angular;
		else
			angularVelocity = TURN_FACTOR * properties_.robot->getMotor().getMaxVelocity().angular;
		properties_.robot->getMotor().setVelocity(Velocity(0, angularVelocity));
	}

	void MovingToMarker::moveTowardsMarker()
	{
		wander_.step();
	}
}
