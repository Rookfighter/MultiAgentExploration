#include <easylogging++.h>
#include "algorithm-compass/AtMarker.hpp"
#include "algorithm-compass/SearchMarker.hpp"

#define FRONT_OBSTACLE_FOV (M_PI / 6) // 30°

namespace mae
{

    AtMarker::AtMarker(const CompassStateProperties &p_properties)
        :CompassState(p_properties),
         movementController_(properties_.robot,
                             properties_.obstacleStopDistance,
                             properties_.obstacleAvoidDistance,
                             properties_.collisionResolveDistance),
         obstacleDetector_(properties_.robot),
         checkingRecommendedDirection_(false)
    {
        LOG(DEBUG) << "Changed to AtMarker state (" << properties_.robot->getName() << ")";

        assert(properties_.currentMarker != NULL);
        if(properties_.lastMarker != NULL) {
            CardinalDirection originDirection = getOppositeDirection(properties_.robot->getCompass().getFacingDirection());
            LOG(DEBUG) << "-- came from marker " << properties_.lastMarker->getID() << " " << getDirectionStr(originDirection) << " (" << properties_.robot->getName() << ")";
            properties_.currentMarker->exploreDirection(originDirection);
        }
    }

    AtMarker::~AtMarker()
    {
    }

    CompassState* AtMarker::update()
    {
        if(!checkingRecommendedDirection_)
            setRecommendedDirection();

        if(checkingRecommendedDirection_ && movementController_.reachedDirection()) {
            if(checkFrontObstacle()) {
                LOG(DEBUG) << "-- direction is blocked (" << properties_.robot->getName() << ")";
                checkingRecommendedDirection_ = false;
            } else {
                LOG(DEBUG) << "-- proceeding to direction " << getDirectionStr(properties_.robot->getCompass().getFacingDirection()) << " (" << properties_.robot->getName() << ")";
                return new SearchMarker(properties_);
            }
        }

        movementController_.update();

        return NULL;
    }

    void AtMarker::setRecommendedDirection()
    {
        assert(properties_.currentMarker != NULL);

        CardinalDirection nextDirection = properties_.currentMarker->getRecommendedDirection();
        CardinalDirection currentDirection = properties_.robot->getCompass().getFacingDirection();

        double angleToTurn = getDirectionDiff(currentDirection, nextDirection) * (M_PI / 2);
        movementController_.turnBy(angleToTurn);

        LOG(DEBUG) << "-- recommended: " << getDirectionStr(nextDirection) << " (" << properties_.robot->getName() << ")";
        LOG(DEBUG) << "-- facing: " << getDirectionStr(currentDirection) << " (" << properties_.robot->getName() << ")";
        LOG(DEBUG) << "-- last timestamp: " << properties_.currentMarker->getDirectionLastVisit(nextDirection) << "ms" << " (" << properties_.robot->getName() << ")";

        properties_.currentMarker->exploreDirection(nextDirection);
        checkingRecommendedDirection_ = true;
    }

    bool AtMarker::checkFrontObstacle()
    {
        assert(properties_.robot->getMarkerSensor().getMaxRange() <= properties_.robot->getRanger().getProperties().getMaxRange());

        bool result = obstacleDetector_.check(-FRONT_OBSTACLE_FOV / 2,
                                              FRONT_OBSTACLE_FOV / 2,
                                              properties_.robot->getMarkerSensor().getMaxRange());

        return result;
    }
}
