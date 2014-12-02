#include <easylogging++.h>
#include "algorithm-rt/SelectingTarget.hpp"
#include "algorithm-rt/UpdatingValue.hpp"
#include "utils/Math.hpp"
#include "utils/Convert.hpp"
#include "utils/Random.hpp"

#define BLOCK_OBSTACLE_FOV (M_PI / 3) // 60°
#define BLOCK_MARKER_FOV (M_PI / 2) // 90°

#define MARKER_OBSTACLE_FOV (M_PI / 4) // 45°

#define MARKER_VALUE_EPS 0.01

namespace mae
{
    // directions for 45° FOV
    /*static const std::vector<double> checkDirections = {
            0               , (M_PI / 4),
            (M_PI / 2)      , (3 * M_PI / 4),
            (M_PI)          , (5 * M_PI / 4),
            (3 * M_PI / 2)  , (7 * M_PI / 4)
    };*/

    // directions for 60° FOV
    static const std::vector<double> checkObstacleDirections = {
                0               , (M_PI / 3),
                (2 * M_PI / 3)  , (M_PI),
                (4 * M_PI / 3)  , (5 * M_PI / 3)
    };

    static const std::vector<double> checkMarkerDirections = {
                    0       , (M_PI / 2),
                    (M_PI)  , (3 * M_PI / 2),
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

        // check if obstacles block any directions
        std::vector<bool> blockedByObstacle(checkObstacleDirections.size());
        for(unsigned int i = 0; i < checkObstacleDirections.size(); ++i) {
            double beginAngle = checkObstacleDirections[i] - (BLOCK_OBSTACLE_FOV / 2);
            double endAngle = checkObstacleDirections[i] + (BLOCK_OBSTACLE_FOV / 2);
            blockedByObstacle[i] = obstacleDetector_.check(beginAngle,
                                                           endAngle,
                                                           obstacleMarkerDistance_);
        }

        // check if marker block any direction
        std::vector<bool> blockedByMarker(checkMarkerDirections.size());
        for(MarkerMeasurement measurement : markerInRange_) {
            for(unsigned int i = 0; i < checkObstacleDirections.size(); ++i) {
                if(!blockedByMarker[i]) {
                    double beginAngle = checkMarkerDirections[i] - (BLOCK_MARKER_FOV / 2);
                    double endAngle = checkMarkerDirections[i] + (BLOCK_MARKER_FOV / 2);
                    blockedByMarker[i] = angleIsBetween(measurement.relativeDirection,
                                                        beginAngle,
                                                        endAngle);
                }
            }
        }

        // mark the directions as blocked, if marker blocks that direction
        for(unsigned int i = 0; i < checkMarkerDirections.size(); ++i) {
            if(blockedByMarker[i]) {
                double beginAngle = checkMarkerDirections[i] - (BLOCK_MARKER_FOV / 2);
                double endAngle = checkMarkerDirections[i] + (BLOCK_MARKER_FOV / 2);
                for(unsigned int j = 0; j < checkObstacleDirections.size(); ++j) {
                    if(angleIsBetween(checkObstacleDirections[j], beginAngle, endAngle))
                        blockedByObstacle[j] = true;
                }
            }
        }

        std::stringstream ss;
        ss << "-- ";
        for(unsigned int i = 0; i < checkObstacleDirections.size(); ++i)
            ss << radianToDegree(checkObstacleDirections[i]) << "°=" << boolToStr(blockedByObstacle[i]) << " ";
        ss << "(" << properties_.robot->getName() << ")";
        LOG(DEBUG) << ss.str();

        int idx = -1;
        for(unsigned int i = 0; i < checkObstacleDirections.size(); ++i) {
            if(!blockedByObstacle[i]) {
                idx = i;
                break;
            }
        }

        if(idx >= 0) {
            properties_.angleToTurn = checkObstacleDirections[idx];
            LOG(DEBUG) << "-- chosen: " << radianToDegree(properties_.angleToTurn) << "° (" << properties_.robot->getName() << ")";
        }

        return  idx >= 0;
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
            if(isPossibleTarget(markerInRange_[i]) &&
                    sameDouble(markerValue, minValue, MARKER_VALUE_EPS)) {
                LOG(DEBUG) << "-- marker " << markerInRange_[i].marker->getID() << " is possible target (" << properties_.robot->getName() << ")";
                result.push_back(markerInRange_[i]);
            }
        }

        return result;
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
            foundMarker = isPossibleTarget(markerInRange_[nextIdx]);
        }

        if(foundMarker) {
            return properties_.calcValue(properties_.currentMarker, markerInRange_[nextIdx].marker);
        } else
            return -1.0;
    }

    bool SelectingTarget::isPossibleTarget(const MarkerMeasurement &p_marker)
    {
        return !markerIsObstructed(p_marker) &&
               !p_marker.marker->isLocked();
    }

    bool SelectingTarget::markerIsObstructed(const MarkerMeasurement &p_markerMeasurement)
    {
        return obstacleDetector_.check(p_markerMeasurement.relativeDirection - (MARKER_OBSTACLE_FOV / 2),
                                       p_markerMeasurement.relativeDirection + (MARKER_OBSTACLE_FOV / 2),
                                       p_markerMeasurement.relativeDistance.length());
    }


}
