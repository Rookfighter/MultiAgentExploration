#include <easylogging++.h>
#include "algorithm-rt/MovingToDirection.hpp"
#include "algorithm-rt/DroppingMarker.hpp"
#include "algorithm-rt/SelectingTarget.hpp"
#include "utils/Math.hpp"

/* determines how much of the max angular
 * velocity is used */
#define TURN_FACTOR 0.8

/* precision of turn angle check */
#define ANGLE_EPS ((M_PI / 180) * 5) // precision 5°

#define FRONT_ANGLE_BEGIN (-M_PI / 6)
#define FRONT_ANGLE_END (M_PI / 6)

namespace mae
{

    MovingToDirection::MovingToDirection(const AntStateProperties &p_properties)
        : AntState(p_properties),
          movementController_(p_properties.robot,
                              p_properties.obstacleStopDistance,
                              p_properties.obstacleAvoidDistance,
                              p_properties.collisionResolveDistance),
          obstacleDetector_(p_properties.robot)
    {
        LOG(DEBUG) << "Changed to MovingToDirection state (" << properties_.robot->getName() << ")";
        movementController_.setAngleEps(ANGLE_EPS);
        movementController_.setTurnFactor(TURN_FACTOR);
        movementController_.turnBy(properties_.angleToTurn);
        movementController_.wanderDistance(properties_.markerDeployDistance);
        LOG(DEBUG) << "-- moving " << properties_.markerDeployDistance << "m (" << properties_.robot->getName() << ")";
    }

    MovingToDirection::~MovingToDirection()
    {
    }

    AntState* MovingToDirection::update()
    {
        if(movementController_.reachedDirection() &&
                (movementController_.reachedDistance() || hasFrontObstacle())) {
            LOG(DEBUG) << "-- reached distance or had obstacle (" << properties_.robot->getName() << ")";
            properties_.robot->getMotor().stop();
            properties_.currentMarker = NULL;

            if(hasTooCloseMarker()) {
                return new SelectingTarget(properties_);
            } else {
                return new DroppingMarker(properties_);
            }
        }

        movementController_.update();

        return NULL;
    }

    bool MovingToDirection::hasFrontObstacle() const
    {
        return obstacleDetector_.check(FRONT_ANGLE_BEGIN,
                                       FRONT_ANGLE_END,
                                       properties_.obstacleStopDistance);
    }

    bool MovingToDirection::hasTooCloseMarker() const
    {
        return properties_.robot->getMarkerSensor().getClosestMarker().relativeDistance.lengthSQ() <= properties_.markerTooCloseDistance * properties_.markerTooCloseDistance;
    }
}
