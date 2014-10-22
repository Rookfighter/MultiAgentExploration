#include "simulation/MarkerSensor.hpp"
#include "common/Math.hpp"

namespace mae
{
	MarkerMeasurement::MarkerMeasurement()
	{
	}

	MarkerMeasurement::~MarkerMeasurement()
	{
	}

	MarkerSensor::MarkerSensor(const RobotConfig& p_config)
		:simulation_(p_config.simulation), stock_(p_config.stock), robotName_(p_config.name)
	{

	}

	MarkerSensor::~MarkerSensor()
	{

	}

	std::vector<Marker*> MarkerSensor::getMarkerInRange()
	{
		std::vector<Marker*> result;
		result.reserve(stock_->getInUse().size());

		for(Marker *marker : stock_->getInUse()) {
			Vector2 distance = getDistanceTo(marker);
			if(distance.lengthSQ() <= marker->getRange() * marker->getRange())
				result.push_back(marker);
		}

		return result;
	}

	std::vector<MarkerMeasurement> MarkerSensor::measureMarkerInRange()
	{
		std::vector<Marker*> markerInRange  = getMarkerInRange();
		std::vector<MarkerMeasurement> result(markerInRange.size());

		for(int i = 0; i < markerInRange.size(); ++i) {
			result[i].marker = markerInRange[i];
			result[i].distance = getDistanceTo(result[i].marker);
			result[i].angleDiff = getAngleTo(result[i].marker);
		}

		return result;
	}

	Marker* MarkerSensor::getClosestMarker()
	{
		Marker *result = NULL;
		for(Marker *marker : stock_->getInUse()) {
			if(result == NULL || getDistanceTo(result).lengthSQ() > getDistanceTo(marker).lengthSQ())
				result = marker;
		}

		if(getDistanceTo(result).lengthSQ() > result->getRange() * result->getRange())
			result = NULL;

		return result;
	}

	Vector2 MarkerSensor::getDistanceTo(Marker* p_marker)
	{
		// TODO : implement distance to marker direction with some derivation
		Vector2 result;
		result = p_marker->getPose().position - simulation_->getPoseOf(robotName_).position;
		return result;
	}

	double MarkerSensor::getAngleTo(Marker* p_marker)
	{
		// TODO - is this correct simulated -- distance has error, so is it ok
		// to use absolutePosition to determine angle offset?
		Vector2 distance = getDistanceTo(p_marker);
		double result = atan2(distance.y, distance.x) - simulation_->getPoseOf(robotName_).yaw;
		result = normalizeRadian(result);
		return result;
	}


}
