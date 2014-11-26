#include <easylogging++.h>
#include "algorithm-rt/SelectingTarget.hpp"
#include "algorithm-rt/UpdatingValue.hpp"
#include "utils/Math.hpp"
#include "utils/Convert.hpp"
#include "utils/Random.hpp"

#define FRONT_IDX 0
#define BACK_IDX 2
#define LEFT_IDX 4
#define RIGHT_IDX 6

#define MARKER_OBSTACLE_FOV (M_PI / 4) // 45°

#define MARKER_VALUE_EPS 0.01

namespace mae
{
    static const double obstacleAngles[8] = {
        -M_PI / 6, M_PI / 6, // FRONT -30°, 30°
        5 * M_PI / 6, -5 * M_PI / 6, // BACK 150°, -150°
        2 * M_PI / 6, 4 * M_PI / 6, // LEFT 60°, 120°
        -4 * M_PI / 6, -2 * M_PI / 6 // RIGHT -120°, -60°
    };

    static const double markerAngles[8] = {
        -M_PI / 4, M_PI / 4, // FRONT -45°, 45°
        3 * M_PI / 4, -3 * M_PI / 4, // BACK 135°, -135°
        M_PI / 4, 3 * M_PI / 4, // LEFT 45°, 135°
        -3 * M_PI / 4, -M_PI / 4 // RIGHT -135°, -46°
    };


    SelectingTarget::SelectingTarget(const AntStateProperties &p_properties)
        : AntState(p_properties),
          markerInRange_(),
          obstacleDetector_(p_properties.robot),
          obstacleMarkerDistance_(p_properties.obstacleMarkerDistance)

    {
        LOG(DEBUG) << "Changed to SelectingTarget state (" << properties_.robot->getName() << ")";
        if(properties_.currentMarker != NULL)
            LOG(DEBUG) << "-- current marker " << properties_.currentMarker->getID() << " (" << properties_.robot->getName() << ")";
        else
            LOG(DEBUG) << "-- current marker is NULL (" << properties_.robot->getName() << ")";
    }

    SelectingTarget::~SelectingTarget()
    {
    }

    AntState* SelectingTarget::update()
    {
        if(properties_.nextMarker != NULL)
            properties_.nextMarker->releaseAsTarget();
        properties_.nextMarker = NULL;
        properties_.angleToTurn = 0;

        getMarkerInRange();
        // check if there is any direction without marker to take
        if(!checkBlankSpace()) {
            LOG(DEBUG) << "-- no blank found (" << properties_.robot->getName() << ")";
            // check if we can find any marker
            if(findNextMarker()) {
                properties_.nextMarker->setAsTarget();
                LOG(DEBUG) << "-- found next marker " << properties_.nextMarker->getID() << " (" << properties_.robot->getName() << ")";
            } else
                LOG(DEBUG) << "-- no fitting next marker found (" << properties_.robot->getName() << ")";
        } else
            LOG(DEBUG) << "-- found blank (" << properties_.robot->getName() << ")";

        return new UpdatingValue(properties_);
    }

    void SelectingTarget::getMarkerInRange()
    {
        markerInRange_.clear();
        markerInRange_ = properties_.robot->getMarkerSensor().getMarkerInRange();

        // find the current marker in the list and remove it
        if(properties_.currentMarker != NULL) {
            std::vector<MarkerMeasurement>::iterator it;
            for(it = markerInRange_.begin(); it != markerInRange_.end(); ++it) {
                if(properties_.currentMarker->getID() == it->marker->getID()) {
                    LOG(DEBUG) << "-- found current marker " << properties_.currentMarker->getID() << " in range (" << properties_.robot->getName() << ")";
                    break;
                }
            }
            markerInRange_.erase(it);
        }

        LOG(DEBUG) << "-- found " << markerInRange_.size() << " marker in range (" << properties_.robot->getName() << ")";
        for(MarkerMeasurement measurement : markerInRange_)
            LOG(DEBUG) << "--> id: " << measurement.marker->getID() << " value: " << measurement.marker->getValue() << " (" << properties_.robot->getName() << ")";
    }

    bool SelectingTarget::checkBlankSpace()
    {
        // check if obstacle is the direction
        bool blockedFront = obstacleDetector_.check(obstacleAngles[FRONT_IDX],
                            obstacleAngles[FRONT_IDX + 1],
                            obstacleMarkerDistance_);
        bool blockedBack = obstacleDetector_.check(obstacleAngles[BACK_IDX],
                           obstacleAngles[BACK_IDX + 1],
                           obstacleMarkerDistance_);
        bool blockedLeft = obstacleDetector_.check(obstacleAngles[LEFT_IDX],
                           obstacleAngles[LEFT_IDX + 1],
                           obstacleMarkerDistance_);
        bool blockedRight = obstacleDetector_.check(obstacleAngles[RIGHT_IDX],
                            obstacleAngles[RIGHT_IDX + 1],
                            obstacleMarkerDistance_);

        LOG(DEBUG) << "-- obstacles: F=" << boolToStr(blockedFront) << ", B=" <<  boolToStr(blockedBack) <<
                   ", L=" << boolToStr(blockedLeft) << ", R=" << boolToStr(blockedRight);
        // check if there is already a marker in that direction
        for(MarkerMeasurement measurement : markerInRange_) {
            if(!blockedFront)
                blockedFront = angleIsBetween(measurement.relativeDirection,
                                              markerAngles[FRONT_IDX],
                                              markerAngles[FRONT_IDX + 1]);

            if(!blockedBack)
                blockedBack = angleIsBetween(measurement.relativeDirection,
                                             markerAngles[BACK_IDX],
                                             markerAngles[BACK_IDX +1]);

            if(!blockedLeft)
                blockedLeft = angleIsBetween(measurement.relativeDirection,
                                             markerAngles[LEFT_IDX],
                                             markerAngles[LEFT_IDX + 1]);

            if(!blockedRight)
                blockedRight = angleIsBetween(measurement.relativeDirection,
                                              markerAngles[RIGHT_IDX],
                                              markerAngles[RIGHT_IDX + 1]);
        }

        LOG(DEBUG) << "-- marker & obstacles: F=" << boolToStr(blockedFront) << ", B=" <<  boolToStr(blockedBack) <<
                   ", L=" << boolToStr(blockedLeft) << ", R=" << boolToStr(blockedRight);

        if(!blockedFront)
            properties_.angleToTurn = 0; // move forward
        else if(!blockedLeft)
            properties_.angleToTurn = M_PI / 2; // turn to left
        else if(!blockedRight)
            properties_.angleToTurn = -M_PI / 2; // turn right
        else if(!blockedBack)
            properties_.angleToTurn = M_PI; // turn back

        return  !blockedFront || !blockedBack || !blockedLeft || !blockedRight;
    }

    bool SelectingTarget::findNextMarker()
    {
        assert(properties_.robot->getMarkerSensor().getMaxRange() <
               properties_.robot->getRanger().getProperties().getMaxRange());

        if(markerInRange_.empty())
            return false;

        std::vector<MarkerMeasurement> possibleTargets = getPossibleTargets();

        if(possibleTargets.empty())
            return false;
        Random rand;
        int idx = rand.nextInt(possibleTargets.size());
        properties_.nextMarker = possibleTargets[idx].marker;

        return true;
    }

    std::vector<MarkerMeasurement> SelectingTarget::getPossibleTargets()
    {
        std::vector<MarkerMeasurement> result;
        double minValue = getMinNonObstructedMarkerValue();
        LOG(DEBUG) << "-- min marker value: " << minValue << " (" << properties_.robot->getName() << ")";

        if(minValue < 0)
            return result;

        for(unsigned int i = 0; i < markerInRange_.size(); ++i) {
            double markerValue = properties_.calcValue(properties_.currentMarker, markerInRange_[i].marker);
            LOG(DEBUG) << "-- algo-value of marker " << markerInRange_[i].marker->getID() << ": " << markerValue << " (" << properties_.robot->getName() << ")";
            if(isPossibleTarget(markerInRange_[i], minValue)) {
                LOG(DEBUG) << "-- marker " << markerInRange_[i].marker->getID() << " is possible target (" << properties_.robot->getName() << ")";
                result.push_back(markerInRange_[i]);
            }
        }

        return result;
    }

    bool SelectingTarget::isPossibleTarget(MarkerMeasurement p_marker,
                                           const double p_minValue)
    {
        double markerValue = properties_.calcValue(properties_.currentMarker, p_marker.marker);
        return sameDouble(markerValue, p_minValue, MARKER_VALUE_EPS) &&
               !markerIsObstructed(p_marker) &&
               !p_marker.marker->isLocked();
    }

    double SelectingTarget::getMinNonObstructedMarkerValue()
    {
        bool foundMarker = false;
        int nextIdx;
        bool alreadyChecked[markerInRange_.size()];
        unsigned int checkedCount = 0;

        for(unsigned int i = 0; i < markerInRange_.size(); ++i)
            alreadyChecked[i] = false;

        while(!foundMarker && checkedCount < markerInRange_.size()) {
            nextIdx = -1;
            // search marker with lowest value
            for(unsigned int i = 0; i < markerInRange_.size(); ++i) {
                if(alreadyChecked[i])
                    continue;
                if(nextIdx == -1)
                    nextIdx = i;
                else {
                    double currentMarkerValue = properties_.calcValue(properties_.currentMarker, markerInRange_[i].marker);
                    double lowestMarkerValue = properties_.calcValue(properties_.currentMarker, markerInRange_[nextIdx].marker);
                    if(lowestMarkerValue > currentMarkerValue)
                        nextIdx = i;
                }
            }
            // exclude this marker from future iterations
            alreadyChecked[nextIdx] = true;
            checkedCount++;

            // check if the way to the marker is blocked by an obstacle
            // and if marker is already locked
            foundMarker = !markerIsObstructed(markerInRange_[nextIdx]) &&
                          !markerInRange_[nextIdx].marker->isLocked();
        }

        if(foundMarker) {
            return properties_.calcValue(properties_.currentMarker, markerInRange_[nextIdx].marker);
        } else
            return -1.0;
    }

    bool SelectingTarget::markerIsObstructed(const MarkerMeasurement &p_markerMeasurement)
    {
        return obstacleDetector_.check(p_markerMeasurement.relativeDirection - (MARKER_OBSTACLE_FOV / 2),
                                       p_markerMeasurement.relativeDirection + (MARKER_OBSTACLE_FOV / 2),
                                       p_markerMeasurement.relativeDistance.length());
    }


}
