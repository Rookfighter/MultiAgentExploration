#ifndef MAE_MARKER_SENSOR_HPP
#define MAE_MARKER_SENSOR_HPP

#include <list>
#include "simulation/RobotConfig.hpp"

namespace mae
{
	struct MarkerMeasurement {
		Vector2 relativeDistance;
		double relativeDirection;
		Marker* marker;
	};

	class MarkerSensor
	{
	private:
		PlayerCc::Graphics2dProxy graphics_;
		
		Simulation *simulation_;
		MarkerStock *stock_;
		
		double range_;

		Pose robotPose_;
		
		MarkerMeasurement getMeasurementFor(Marker* p_marker);
		void drawPolygon();
	public:
		MarkerSensor(const RobotConfig& p_config);
		~MarkerSensor();

		void setRobotPose(const Pose &p_pose);

		std::vector<MarkerMeasurement> getMarkerInRange();
		MarkerMeasurement getClosestMarker();
		
		double getRange() const;

	};

}

#endif
