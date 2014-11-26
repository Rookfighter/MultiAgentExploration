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

/* determines how much of marker range should
 * be moved until attempt to reach marker is
 * cancelled */
#define DISTANCE_FACTOR 1.1

#define OBSTACLE_DETECT_FOV (M_PI / 3) // 60°

#define OBSTACLE_AVOID_MAX_STEP 5

namespace mae
{

    MovingToMarker::MovingToMarker(const AntStateProperties &p_properties)
        :AntState(p_properties),
         movementController_(p_properties.robot,
                             p_properties.obstacleStopDistance,
                             p_properties.obstacleAvoidDistance,
                             p_properties.collisionResolveDistance),
         obstacleDetector_(p_properties.robot),
         foundMarker_(false),
         obstacleAvoidStep_(0)
    {
        movementController_.setAngleEps(ANGLE_EPS);
        movementController_.setTurnFactor(TURN_FACTOR);
        movementController_.wanderDistance(DISTANCE_FACTOR * properties_.robot->getMarkerSensor().getMaxRange());

        LOG(DEBUG) << "Changed to MovingToMarker state (" << properties_.robot->getName() << ")";
        LOG(DEBUG) << "-- target marker " << properties_.nextMarker->getID() << " (" << properties_.robot->getName() << ")";
        LOG(DEBUG) << "-- maximum wander distance " << DISTANCE_FACTOR * properties_.robot->getMarkerSensor().getMaxRange() << "m (" << properties_.robot->getName() << ")";
    }

    MovingToMarker::~MovingToMarker()
    {
    }

    AntState* MovingToMarker::update()
    {
        updateTargetMeasurement();

        if(!foundMarker_) {
            LOG(DEBUG) << "-- signal to target marker " << properties_.nextMarker->getID() << " lost (" << properties_.robot->getName() << ")";
            properties_.currentMarker = NULL;
            properties_.robot->getMotor().stop();
            return new SelectingTarget(properties_);
        }

        if(reachedTarget()) {
            LOG(DEBUG) << "-- reached target marker " << properties_.nextMarker->getID() << " (" << properties_.robot->getName() << ")";
            // reached the marker, now select next target
            properties_.currentMarker = properties_.nextMarker;
            properties_.robot->getMotor().stop();
            return new SelectingTarget(properties_);
        }
        if(movementController_.reachedDistance()) {
            LOG(DEBUG) << "-- moved too far, not reached marker " << properties_.nextMarker->getID() << " (" << properties_.robot->getName() << ")";
            // moved far, but did not reach marker
            if(hasOtherMarkersInRange()) {
                LOG(DEBUG) << "-- have other marker in range (" << properties_.robot->getName() << ")";
                properties_.currentMarker = NULL;
                properties_.robot->getMotor().stop();
                return new SelectingTarget(properties_);
            } else {
                LOG(DEBUG) << "-- no other marker in range (" << properties_.robot->getName() << ")";
                properties_.robot->getMotor().stop();
                return new DroppingMarker(properties_);
            }
        }

        if(hasObstacleToTarget()) {
            obstacleAvoidStep_ = OBSTACLE_AVOID_MAX_STEP;
        }

        // if we are not avoiding any obstacles turn to the target
        if(!isAvoidingObstacle() && movementController_.reachedDirection() && !isFacingToTarget())
            turnToTarget();

        move();
        
        return NULL;
    }

    void MovingToMarker::updateTargetMeasurement()
    {
        markerInRange_ = properties_.robot->getMarkerSensor().getMarkerInRange();

        // find target marker from all available marker
        // check if marker was not found (is destroyed or not in range anymore)
        foundMarker_ = false;
        for(MarkerMeasurement measurement : markerInRange_) {
            if(properties_.nextMarker->getID() == measurement.marker->getID()) {
                targetMeasurement_ = measurement;
                foundMarker_ = true;
                break;
            }
        }
    }

    bool MovingToMarker::reachedTarget()
    {
        return sameDouble(targetMeasurement_.relativeDistance.length(),
                          0,
                          DISTANCE_EPS);
    }


    bool MovingToMarker::isFacingToTarget()
    {
        return sameDouble(0,
                          targetMeasurement_.relativeDirection,
                          ANGLE_EPS);
    }
    
    bool MovingToMarker::hasOtherMarkersInRange()
    {
        std::vector<MarkerMeasurement> markerInRangeTmp = markerInRange_;
        std::vector<MarkerMeasurement>::iterator it;
        bool found;
        
        // find currentMarker
        if(properties_.currentMarker != NULL) {
            found = false;
            for(it = markerInRangeTmp.begin(); it != markerInRangeTmp.end(); ++it) {
                if(it->marker->getID() == properties_.currentMarker->getID()) {
                    found = true;
                    break;
                }
            }
            if(found)
                markerInRangeTmp.erase(it);
        }
        
        // find nextMarker
        found = false;
        for(it = markerInRangeTmp.begin(); it != markerInRangeTmp.end(); ++it) {
            if(it->marker->getID() == properties_.nextMarker->getID()) {
                found = true;
                break;
            }
        }
        if(found)
            markerInRangeTmp.erase(it);
        
        return !markerInRangeTmp.empty();
    }

    bool MovingToMarker::isAvoidingObstacle()
    {
        return obstacleAvoidStep_ != 0;
    }

    bool MovingToMarker::hasObstacleToTarget()
    {
        return obstacleDetector_.check(targetMeasurement_.relativeDirection - (OBSTACLE_DETECT_FOV / 2),
                                       targetMeasurement_.relativeDirection + (OBSTACLE_DETECT_FOV / 2),
                                       std::min(properties_.obstacleAvoidDistance, properties_.obstacleStopDistance));
    }

    void MovingToMarker::turnToTarget()
    {
        movementController_.turnBy(targetMeasurement_.relativeDirection);
    }

    void MovingToMarker::move()
    {
        movementController_.update();

        // we avoided the obstalce for one step
        if(isAvoidingObstacle())
            obstacleAvoidStep_--;
    }
}
