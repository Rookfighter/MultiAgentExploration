#include <easylogging++.h>
#include "algorithm-rt/MovingToDirection.hpp"
#include "algorithm-rt/DroppingMarker.hpp"
#include "utils/Math.hpp"

/* determines how much of the max angular
 * velocity is used */
#define TURN_FACTOR 0.8

/* determines how much of marker range should
 * be moved until we place new marker */
#define DISTANCE_FACTOR 0.8

/* precision of turn angle check */
#define ANGLE_EPS ((M_PI / 180) * 5) // precision 5°

#define FRONT_ANGLE_BEGIN (-M_PI / 6)
#define FRONT_ANGLE_END (M_PI / 6)

namespace mae
{

	MovingToDirection::MovingToDirection(const AntStateProperties &p_properties)
		: AntState(p_properties),
		  movementController_(p_properties.robot, p_properties.obstacleAvoidDistance),
		  obstacleDetector_(p_properties.robot)
	{
		LOG(DEBUG) << "Changed to MovingToDirection state";
		movementController_.setAngleEps(ANGLE_EPS);
		movementController_.setTurnFactor(TURN_FACTOR);
		movementController_.turnBy(properties_.angleToTurn);
		movementController_.wanderDistance(DISTANCE_FACTOR * properties_.robot->getMarkerSensor().getMaxRange());
	}

	MovingToDirection::~MovingToDirection()
	{
	}

	AntState* MovingToDirection::update()
	{
		if(movementController_.finished() || hasFrontObstacle()) {
			properties_.robot->getMotor().stop();
			return new DroppingMarker(properties_);
		}
		
		movementController_.update();

		return NULL;
	}

	bool MovingToDirection::hasFrontObstacle()
	{
		return obstacleDetector_.check(FRONT_ANGLE_BEGIN,
		                               FRONT_ANGLE_END,
		                               properties_.obstacleAvoidDistance);
	}

}
