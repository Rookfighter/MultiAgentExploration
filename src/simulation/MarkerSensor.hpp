#ifndef MAE_MARKER_SENSOR_HPP
#define MAE_MARKER_SENSOR_HPP

#include "simulation/RobotConfig.hpp"

namespace mae
{
	class MarkerMeasurement
	{
	public:
		Marker* marker;
		Vector2 distance;
		double angleDiff;
		
		MarkerMeasurement();
		~MarkerMeasurement();
	};
	
	class MarkerSensor
	{
	private:
		Simulation *simulation_;
		MarkerStock *stock_;
		
		std::string robotName_;
		
	public:
		MarkerSensor(const RobotConfig& p_config);
		~MarkerSensor();
		
		std::vector<Marker*> getMarkerInRange();
		std::vector<MarkerMeasurement> measureMarkerInRange();
		Marker* getClosestMarker();
		
		Vector2 getDistanceTo(Marker* p_marker);
		double getAngleTo(Marker* p_marker);
		
	};

}

#endif
