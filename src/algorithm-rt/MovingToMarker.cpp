#include <easylogging++.h>
#include "algorithm-rt/MovingToMarker.hpp"
#include "algorithm-rt/SelectingTarget.hpp"
#include "algorithm-rt/DroppingMarker.hpp"
#include "utils/Math.hpp"

/* avoid moving in a direction that is farer
 * away than this (+/-). */
#define ANGLE_EPS (M_PI / 18) // 10°

/* determines how much of the max angular
 * velocity is used */
#define TURN_FACTOR 0.8

/* determines how much of marker range is
 * enough to be close enough*/
#define DISTANCE_EPS 0.20

/* determines how much fo marker range should
 * be moved until attempt to reach marker is
 * cancelled */
#define MOVEMENT_FACTOR 1.1

#define OBSTACLE_DETECT_FOV (M_PI / 3) // 60°

#define OBSTACLE_AVOID_MAX_STEP 5

namespace mae
{

	MovingToMarker::MovingToMarker(const AntStateProperties &p_properties)
		:AntState(p_properties),
		 wander_(p_properties.robot, p_properties.obstacleAvoidDistance),
		 obstacleDetector_(p_properties.robot),
		 lastPose_(p_properties.robot->getMotor().getPose()),
		 movedDistance_(0),
		 obstacleAvoidStep_(0)
	{
		LOG(DEBUG) << "Changed to MovingToMarker state";
	}

	MovingToMarker::~MovingToMarker()
	{
	}

	AntState* MovingToMarker::update()
	{
		updateGeometry();

		if(!foundMarker_) {
			LOG(DEBUG) << "-- signal to marker lost";
			properties_.currentMarker = NULL;
			properties_.robot->getMotor().stop();
			return new SelectingTarget(properties_);
		}

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
		if(hasObstacleToTarget()) {
			obstacleAvoidStep_ = OBSTACLE_AVOID_MAX_STEP;
		}

		// no target reached, we still have to move
		if(!isAvoidingObstacle() && !reachedDirection())
			turnToMarker();
		else
			wander();

		return NULL;
	}

	void MovingToMarker::updateGeometry()
	{
		// get moved distance since last call
		Pose currentPose = properties_.robot->getMotor().getPose();
		movedDistance_ += (currentPose.position - lastPose_.position).length();
		lastPose_ = currentPose;

		// get measurement to target Marker e.g. rel. distance, rel. direction
		updateTargetMeasurement();
	}

	void MovingToMarker::updateTargetMeasurement()
	{
		std::vector<MarkerMeasurement> markerInRange = properties_.robot->getMarkerSensor().getMarkerInRange();

		// find target marker from all available marker
		// if marker was not found (is destroyed or not in range anymore)
		// we just keep wandering
		foundMarker_ = false;
		for(MarkerMeasurement measurement : markerInRange) {
			if(properties_.nextMarker->getID() == measurement.marker->getID()) {
				targetMeasurement_ = measurement;
				foundMarker_ = true;
				break;
			}
		}
	}

	bool MovingToMarker::reachedDirection()
	{
		return sameDouble(targetMeasurement_.relativeDirection,
		                  0,
		                  ANGLE_EPS);
	}

	bool MovingToMarker::reachedTarget()
	{
		return sameDouble(targetMeasurement_.relativeDistance.length(),
		                  0,
		                  DISTANCE_EPS);
	}

	bool MovingToMarker::movedEnough()
	{
		return movedDistance_ >= MOVEMENT_FACTOR * properties_.robot->getMarkerSensor().getMaxRange();
	}


	void MovingToMarker::turnToMarker()
	{
		double angularVelocity;
		if(targetMeasurement_.relativeDirection < 0)
			angularVelocity = TURN_FACTOR * properties_.robot->getMotor().getMinVelocity().angular;
		else
			angularVelocity = TURN_FACTOR * properties_.robot->getMotor().getMaxVelocity().angular;
		properties_.robot->getMotor().setVelocity(Velocity(0, angularVelocity));
	}

	void MovingToMarker::wander()
	{
		wander_.update();
		
		// we avoided the obstalce for one step
		if(isAvoidingObstacle())
			obstacleAvoidStep_--;
	}

	bool MovingToMarker::isAvoidingObstacle()
	{
		return obstacleAvoidStep_ != 0;
	}
	
	bool MovingToMarker::hasObstacleToTarget()
	{
		return obstacleDetector_.check(targetMeasurement_.relativeDirection - (OBSTACLE_DETECT_FOV / 2),
		                               targetMeasurement_.relativeDirection + (OBSTACLE_DETECT_FOV / 2),
		                               properties_.obstacleAvoidDistance);
	}
}
