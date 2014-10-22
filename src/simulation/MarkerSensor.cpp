#include "simulation/MarkerSensor.hpp"

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
		result.reserve(stock_.getAvailable().size());

		for(Marker *marker : stock_.getAvailable()) {
			Vector2 distance = getDistanceTo(marker);
			if(distance.lengthSQ() <= marker->range * marker->range)
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
	
	Vector2 MarkerSensor::getDistanceTo(Marker* p_marker)
	{
		// TODO : implement distance to marker direction with some derivation
		Vector2 result;
		result = p_marker->pose.position - simulation_.getPoseOf(robotName_).position;
		return result;
	}

	double MarkerSensor::getAngleTo(Marker* p_marker)
	{
		// TODO - is this correct simulated -- distance has error, so is it ok
		// to use absolutePosition to determine angle offset?
		Vector2 distance = getDistanceTo(p_marker);
		double result = atan2(distance.y, distance.x) - simulation_.getPoseOf(robotName_).yaw;
		result = normalizeRadian(result);
		return result;
	}


}
