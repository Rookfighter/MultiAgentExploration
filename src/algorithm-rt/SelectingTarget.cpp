#include <easylogging++.h>
#include "algorithm-rt/SelectingTarget.hpp"
#include "algorithm-rt/UpdatingValue.hpp"
#include "utils/Math.hpp"
#include "utils/Convert.hpp"
#include "utils/Random.hpp"

#define BLOCK_MARKER_FOV (M_PI / 2) // 90°
#define MARKER_OBSTACLE_FOV (M_PI / 4) // 45°
#define RANDOM_DIRECTION (M_PI / 4) // 45°

#define MARKER_VALUE_EPS 0.01

/* precision of turn angle check */
#define ANGLE_EPS ((M_PI / 180) * 5) // precision 5°

namespace mae
{

    SelectingTarget::SelectingTarget(const AntStateProperties &p_properties)
        : AntState(p_properties),
          markerInRange_(),
          directionsInfos_(),
          state_(Init),
          nextDirectionIdx_(-1),
          obstacleDetector_(p_properties.robot),
          movementController_(p_properties.robot,
                              p_properties.obstacleStopDistance,
                              p_properties.obstacleAvoidDistance,
                              p_properties.collisionResolveDistance),
          obstacleMarkerDistance_(p_properties.obstacleMarkerDistance),
          facingDirection_(0)

    {
        LOG(DEBUG) << "Changed to SelectingTarget state (" << properties_.robot->getName() << ")";
        if(properties_.currentMarker != NULL)
            LOG(DEBUG) << "-- current marker " << properties_.currentMarker->getID() << " (" << properties_.robot->getName() << ")";
        else
            LOG(DEBUG) << "-- current marker is NULL (" << properties_.robot->getName() << ")";

        movementController_.setAngleEps(ANGLE_EPS);
        initDirectionInfos();
    }

    void SelectingTarget::initDirectionInfos()
    {
        directionsInfos_.resize(12);

        // front sensors
        directionsInfos_[0].direction = degreeToRadian(0);
        directionsInfos_[0].sensordIdx.resize(2);
        directionsInfos_[0].sensordIdx[0] = Ranger::SENSOR_N10;
        directionsInfos_[0].sensordIdx[1] = Ranger::SENSOR_P10;

        // left 30° sensor
        directionsInfos_[1].direction = degreeToRadian(30);
        directionsInfos_[1].sensordIdx.resize(1);
        directionsInfos_[1].sensordIdx[0] = Ranger::SENSOR_P30;

        // right 30° sensor
        directionsInfos_[2].direction = degreeToRadian(-30);
        directionsInfos_[2].sensordIdx.resize(1);
        directionsInfos_[2].sensordIdx[0] = Ranger::SENSOR_N30;

        // left 50° sensor
        directionsInfos_[3].direction = degreeToRadian(50);
        directionsInfos_[3].sensordIdx.resize(1);
        directionsInfos_[3].sensordIdx[0] = Ranger::SENSOR_P50;

        //right 50° sensor
        directionsInfos_[4].direction = degreeToRadian(-50);
        directionsInfos_[4].sensordIdx.resize(1);
        directionsInfos_[4].sensordIdx[0] = Ranger::SENSOR_N50;

        // left 90° sensors
        directionsInfos_[5].direction = degreeToRadian(90);
        directionsInfos_[5].sensordIdx.resize(2);
        directionsInfos_[5].sensordIdx[0] = Ranger::SENSOR_P90F;
        directionsInfos_[5].sensordIdx[1] = Ranger::SENSOR_P90B;

        // right 90° sensors
        directionsInfos_[6].direction = degreeToRadian(-90);
        directionsInfos_[6].sensordIdx.resize(2);
        directionsInfos_[6].sensordIdx[0] = Ranger::SENSOR_N90F;
        directionsInfos_[6].sensordIdx[1] = Ranger::SENSOR_N90B;

        // left 130° sensor
        directionsInfos_[7].direction = degreeToRadian(130);
        directionsInfos_[7].sensordIdx.resize(1);
        directionsInfos_[7].sensordIdx[0] = Ranger::SENSOR_P130;

        // right 130° sensor
        directionsInfos_[8].direction = degreeToRadian(-130);
        directionsInfos_[8].sensordIdx.resize(1);
        directionsInfos_[8].sensordIdx[0] = Ranger::SENSOR_N130;

        // left 150° sensor
        directionsInfos_[9].direction = degreeToRadian(150);
        directionsInfos_[9].sensordIdx.resize(1);
        directionsInfos_[9].sensordIdx[0] = Ranger::SENSOR_P150;

        // right 150° sensor
        directionsInfos_[10].direction = degreeToRadian(-150);
        directionsInfos_[10].sensordIdx.resize(1);
        directionsInfos_[10].sensordIdx[0] = Ranger::SENSOR_N150;

        //back sensors
        directionsInfos_[11].direction = degreeToRadian(180);
        directionsInfos_[11].sensordIdx.resize(2);
        directionsInfos_[11].sensordIdx[0] = Ranger::SENSOR_P170;
        directionsInfos_[11].sensordIdx[1] = Ranger::SENSOR_N170;

        for(unsigned int i = 0; i < directionsInfos_.size(); ++i)
            directionsInfos_[i].blocked = false;
    }

    SelectingTarget::~SelectingTarget()
    {
    }

    AntState* SelectingTarget::update()
    {
        State oldState;
        AntState *result;

        oldState = state_;
        result = executeState();
        while(result == NULL && oldState != state_) {
            oldState = state_;
            result = executeState();
        }

        return result;
    }

    AntState* SelectingTarget::executeState()
    {
        switch(state_) {
        case Init:
            if(properties_.nextMarker != NULL) {
                properties_.nextMarker->releaseAsTarget();
                properties_.nextMarker = NULL;
            }

            state_ = CheckingSurrounding;
            break;
        case CheckingSurrounding:
            checkSourrounding();
            state_ = SelectingBlank;
            break;
        case SelectingBlank:
            selectBlankSpace();
            if(foundBlankSpace()) {
                turnToBlank();
                state_ = TurningToBlank;
            } else {
                state_ = SelectingMarker;
            }
            break;
        case TurningToBlank:
            if(movementController_.reachedDirection()) {
                state_ = CheckingFront;
            } else {
                movementController_.update();
            }
            break;
        case CheckingFront:
            if(!obstacleDetector_.checkFront(obstacleMarkerDistance_)) {
                // found a blank space and front is not blocked
                LOG(DEBUG) << "-- found blank in " << radianToDegree(directionsInfos_[nextDirectionIdx_].direction) << "°";
                return new UpdatingValue(properties_);
            } else {
                // front is blocked
                LOG(DEBUG) << "-- front (" << radianToDegree(directionsInfos_[nextDirectionIdx_].direction) << "°)" << " is blocked";
                directionsInfos_[nextDirectionIdx_].blocked = true;
                facingDirection_ = directionsInfos_[nextDirectionIdx_].direction;
                state_ = SelectingBlank;
            }
            break;
        case SelectingMarker:
            checkSourrounding();
            selectNextMarker();
            if(properties_.nextMarker == NULL) {
                LOG(WARNING) << "-- no blank and no next marker found (" << properties_.robot->getName() << ")";
                int randIdx = Random::nextInt(directionsInfos_.size());
                movementController_.turnBy(directionsInfos_[randIdx].direction);
                state_ = TurningToDirection;
            } else {
                LOG(DEBUG) << "-- found target marker: " << properties_.nextMarker->getID();
                properties_.nextMarker->setAsTarget();
                return new UpdatingValue(properties_);
            }
            break;
        case TurningToDirection:
            if(movementController_.reachedDirection()) {
                return new UpdatingValue(properties_);
            } else {
                movementController_.update();
            }
            break;
        }

        return NULL;
    }

    void SelectingTarget::checkSourrounding()
    {
        facingDirection_ = 0;

        getMarkerInRange();
        checkBlankSpace();
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

    void SelectingTarget::checkBlankSpace()
    {
        for(unsigned int i = 0; i < directionsInfos_.size(); ++i)
            directionsInfos_[i].blocked = false;

        // check if obstacles block any direction
        for(unsigned int i = 0; i < directionsInfos_.size(); ++i) {
            if(obstacleDetector_.check(directionsInfos_[i].sensordIdx, obstacleMarkerDistance_))
                directionsInfos_[i].blocked = true;
        }

        // check if marker block any direction
        for(unsigned int i = 0; i < directionsInfos_.size(); ++i) {
            for(MarkerMeasurement measurement : markerInRange_) {
                double beginAngle = measurement.relativeDirection - (BLOCK_MARKER_FOV / 2);
                double endAngle = measurement.relativeDirection + (BLOCK_MARKER_FOV / 2);
                if(angleIsBetween(directionsInfos_[i].direction, beginAngle, endAngle))
                    directionsInfos_[i].blocked = true;
            }
        }

        std::stringstream ss;
        ss << "-- ";
        for(unsigned int i = 0; i < directionsInfos_.size(); ++i)
            ss << radianToDegree(directionsInfos_[i].direction) << "°=" << boolToStr(directionsInfos_[i].blocked) << " ";
        ss << "(" << properties_.robot->getName() << ")";
        LOG(DEBUG) << ss.str();
    }

    void SelectingTarget::selectBlankSpace()
    {
        nextDirectionIdx_ = -1;

        std::vector<unsigned int> possibleDirections;
        for(unsigned int i = 0; i < directionsInfos_.size(); ++i) {
            if(!directionsInfos_[i].blocked)
                possibleDirections.push_back(i);
        }

        if(!possibleDirections.empty()) {
            int randIdx = Random::nextInt(possibleDirections.size());
            nextDirectionIdx_ = possibleDirections[randIdx];
        }
    }

    bool SelectingTarget::foundBlankSpace()
    {
        return nextDirectionIdx_ >= 0;
    }

    void SelectingTarget::turnToBlank()
    {
        double angleToTurn = directionsInfos_[nextDirectionIdx_].direction - facingDirection_;
        movementController_.turnBy(angleToTurn);
    }

    void SelectingTarget::selectNextMarker()
    {
        assert(properties_.robot->getMarkerSensor().getMaxRange() <
               properties_.robot->getRanger().getProperties().getMaxRange());
        properties_.nextMarker = NULL;

        if(markerInRange_.empty())
            return;

        std::vector<MarkerMeasurement> possibleTargets = getPossibleTargets();

        if(possibleTargets.empty())
            return;

        int idx = Random::nextInt(possibleTargets.size());
        properties_.nextMarker = possibleTargets[idx].marker;
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
