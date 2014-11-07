#include <easylogging++.h>
#include "simulation/MarkerSensor.hpp"
#include "utils/Math.hpp"

namespace mae
{
	MarkerSensor::MarkerSensor(const RobotConfig& p_config)
		:stock_(p_config.stock), robotPose_(),
		 maxRange_(p_config.markerSensorMaxRange)
	{
	}

	MarkerSensor::~MarkerSensor()
	{

	}

	void MarkerSensor::setRobotPose(const Pose &p_pose)
	{
		robotPose_ = p_pose;
	}

	std::vector<MarkerMeasurement> MarkerSensor::getMarkerInRange()
	{
		std::vector<MarkerMeasurement> result;
		Vector2 distance;

		result.reserve(stock_->getMarker().size() / 4);
		for(Marker *marker : stock_->getMarker()) {
			distance = marker->getPose().position - robotPose_.position;
			if(distance.lengthSQ() <= maxRange_ * maxRange_)
				result.push_back(getMeasurementFor(marker));
		}

		return result;
	}

	MarkerMeasurement MarkerSensor::getMeasurementFor(Marker* p_marker)
	{
		MarkerMeasurement result;

		result.marker = p_marker;
		// TODO : implement distance to marker and direction with some derivation
		result.relativeDistance = p_marker->getPose().position - robotPose_.position;
		result.relativeDirection = atan2(result.relativeDistance.y, result.relativeDistance.x) -
		                           normalizeRadian(robotPose_.yaw);
		result.relativeDirection = normalizeRadian(result.relativeDirection);

		return result;
	}

	MarkerMeasurement MarkerSensor::getClosestMarker()
	{
		std::vector<MarkerMeasurement> inRange = getMarkerInRange();

		bool initialized = false;
		MarkerMeasurement result;
		for(MarkerMeasurement measurement : inRange) {
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
