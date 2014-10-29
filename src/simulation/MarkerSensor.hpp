#ifndef MAE_MARKER_SENSOR_HPP
#define MAE_MARKER_SENSOR_HPP

#include <list>
#include "simulation/RobotConfig.hpp"

namespace mae
{	
	class MarkerSensor
	{
	private:
		Simulation *simulation_;
		MarkerStock *stock_;
		
		Pose robotPose_;
		
	public:
		MarkerSensor(const RobotConfig& p_config);
		~MarkerSensor();
		
		void setRobotPose(const Pose &p_pose);
		
		void getMarkerInRange(std::list<Marker*> &p_result);
		Marker* getClosestMarker();
		
		Vector2 getDistanceTo(Marker* p_marker);
		double getAngleTo(Marker* p_marker);
		
	};

}

#endif
