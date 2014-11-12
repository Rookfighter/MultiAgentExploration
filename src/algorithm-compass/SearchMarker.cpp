#include <easylogging++.h>
#include "algorithm-compass/SearchMarker.hpp"
#include "algorithm-compass/AtMarker.hpp"
#include "algorithm-compass/DeployMarker.hpp"

#define TURN_FACTOR 0.8

/* precision of turn angle check */
#define ANGLE_EPS ((M_PI / 180) * 5) // precision 5°

namespace mae
{

    SearchMarker::SearchMarker(const CompassStateProperties &p_properties)
        :CompassState(p_properties),
         movementController_(properties_.robot, properties_.obstacleAvoidDistance),
         newMarker_(NULL),
         lostAllSignals_(false)
    {
        LOG(DEBUG) << "Changed to SearchMarker state";
        movementController_.setTurnFactor(TURN_FACTOR);
        movementController_.setAngleEps(ANGLE_EPS);
        movementController_.wanderDistance(properties_.markerDeployDistance);
        
         properties_.lastMarker = properties_.currentMarker;
    }

    SearchMarker::~SearchMarker()
    {
    }

    CompassState* SearchMarker::update()
    {
        updateNewMarker();
        
        if(lostAllSignals_) {
            properties_.currentMarker = NULL;
            properties_.robot->getMotor().stop();
            return new DeployMarker(properties_);
        }
        
        if(newMarker_ != NULL) {
            properties_.currentMarker = newMarker_;
            properties_.robot->getMotor().stop();
            return new AtMarker(properties_);
        }
        
        if(movementController_.reachedDistance())
            movementController_.wanderDistance(properties_.markerDeployDistance);
        
        movementController_.update();
        
        return NULL;
    }
    
    void SearchMarker::updateNewMarker()
    {
        std::vector<MarkerMeasurement> markerInRange = properties_.robot->getMarkerSensor().getMarkerInRange();
        std::vector<MarkerMeasurement> markerWithoutLast;
        markerWithoutLast.reserve(markerInRange.size());
        bool foundCurrent = false;;
        
        lostAllSignals_ = markerInRange.empty();
        
        for(MarkerMeasurement measurement : markerInRange) {
            if(measurement.marker->getID() != properties_.lastMarker->getID())
                markerWithoutLast.push_back(measurement);
            else
                foundCurrent = true;
        }
        
        if(!foundCurrent && !markerWithoutLast.empty()) {
            // only set new marker after we lost signal 
            newMarker_ = properties_.robot->getMarkerSensor().getClosestMarker(markerWithoutLast).marker;
        }
        
    }
}
