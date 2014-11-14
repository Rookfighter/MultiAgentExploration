#include <easylogging++.h>
#include "simulation/MarkerSensor.hpp"
#include "utils/Math.hpp"

namespace mae
{
    MarkerSensor::MarkerSensor(const RobotConfig& p_config)
        :stock_(p_config.stock),
         model_(p_config.world->GetModel(p_config.name)),
         maxRange_(p_config.markerSensorMaxRange)
    {
    }

    MarkerSensor::~MarkerSensor()
    {

    }

    std::vector<MarkerMeasurement> MarkerSensor::getMarkerInRange()
    {
        std::vector<MarkerMeasurement> result;
        Vector2f distance;
        Pose robotPose;
        robotPose.fromStagePose(model_->GetGlobalPose());

        result.reserve(stock_->getMarker().size() / 4);
        for(Marker *marker : stock_->getMarker()) {
            distance = marker->getPose().position - robotPose.position;
            if(distance.lengthSQ() <= maxRange_ * maxRange_)
                result.push_back(getMeasurementFor(marker));
        }

        return result;
    }

    MarkerMeasurement MarkerSensor::getMeasurementFor(Marker* p_marker)
    {
        MarkerMeasurement result;
        Pose robotPose;
        robotPose.fromStagePose(model_->GetGlobalPose());

        result.marker = p_marker;
        // TODO : implement distance to marker and direction with some derivation
        result.relativeDistance = p_marker->getPose().position - robotPose.position;
        result.relativeDirection = atan2(result.relativeDistance.y, result.relativeDistance.x) -
                                   normalizeRadian(robotPose.yaw);
        result.relativeDirection = normalizeRadian(result.relativeDirection);

        return result;
    }

    MarkerMeasurement MarkerSensor::getClosestMarker()
    {
        return getClosestMarker(getMarkerInRange());
    }

    MarkerMeasurement MarkerSensor::getClosestMarker(const std::vector<MarkerMeasurement> &p_marker)
    {
        bool initialized = false;
        MarkerMeasurement result;
        for(MarkerMeasurement measurement : p_marker) {
            if(!initialized ||
                    measurement.relativeDistance.lengthSQ() < result.relativeDistance.lengthSQ()) {
                initialized = true;
                result = measurement;
            }
        }

        return result;
    }

    double MarkerSensor::getMaxRange() const
    {
        return maxRange_;
    }

}
