#include <easylogging++.h>
#include "algorithm-compass/SearchMarker.hpp"
#include "algorithm-compass/AtMarker.hpp"
#include "algorithm-compass/DeployMarker.hpp"

/* determines how much of markersensor distance will
 * be moved until new marker is dropped */
#define DISTANCE_FACTOR 1.8

#define TURN_FACTOR 0.8

/* precision of turn angle check */
#define ANGLE_EPS ((M_PI / 180) * 5) // precision 5°

namespace mae
{

    SearchMarker::SearchMarker(const CompassStateProperties &p_properties)
        :CompassState(p_properties),
         movementController_(properties_.robot, properties_.obstacleAvoidDistance),
         newMarker_(NULL)
    {
        LOG(DEBUG) << "Changed to SearchMarker state";
        movementController_.setTurnFactor(TURN_FACTOR);
        movementController_.setAngleEps(ANGLE_EPS);
        movementController_.wanderDistance(DISTANCE_FACTOR * properties_.robot->getMarkerSensor().getMaxRange());
    }

    SearchMarker::~SearchMarker()
    {
    }

    CompassState* SearchMarker::update()
    {
        updateNewMarker();
        
        if(newMarker_ != NULL)
            return new AtMarker(properties_);
        
        if(movementController_.reachedDistance())
            return new DeployMarker(properties_);
        
        movementController_.update();
        
        return NULL;
    }
    
    void SearchMarker::updateNewMarker()
    {
        std::vector<MarkerMeasurement> markerInRange = properties_.robot->getMarkerSensor().getMarkerInRange();
        std::vector<MarkerMeasurement> markerWithoutCurrent;
        markerWithoutCurrent.reserve(markerInRange.size());
        
        for(MarkerMeasurement measurement : markerInRange) {
            if(measurement.marker->getID() != properties_.currentMarker->getID())
                markerWithoutCurrent.push_back(measurement);
        }
        
        if(markerWithoutCurrent.size() == 1) {
            newMarker_ = markerWithoutCurrent[0].marker;
            properties_.currentMarker = newMarker_;
        } else if(markerWithoutCurrent.size() > 1) {
            int idx = random_.nextInt(markerWithoutCurrent.size());
            newMarker_ = markerWithoutCurrent[idx].marker;
            properties_.currentMarker = newMarker_;
        }
        
    }
}
