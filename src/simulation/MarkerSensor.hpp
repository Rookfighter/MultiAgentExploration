#ifndef MAE_MARKER_SENSOR_HPP
#define MAE_MARKER_SENSOR_HPP

#include <list>
#include "simulation/RobotConfig.hpp"

namespace mae
{
	struct MarkerMeasurement {
		Vector2f relativeDistance;
		double relativeDirection;
		Marker* marker;
	};

	class MarkerSensor
	{
	private:
		MarkerStock *stock_;
        Stg::Model *model_;
        double maxRange_;
		
		MarkerMeasurement getMeasurementFor(Marker* p_marker);
	public:
		MarkerSensor(const RobotConfig& p_config);
		~MarkerSensor();

		std::vector<MarkerMeasurement> getMarkerInRange();
		MarkerMeasurement getClosestMarker();
        MarkerMeasurement getClosestMarker(const std::vector<MarkerMeasurement> &p_marker);
		
		double getMaxRange() const;

	};

}

#endif
