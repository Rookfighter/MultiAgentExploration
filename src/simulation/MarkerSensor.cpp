#include <easylogging++.h>
#include "simulation/MarkerSensor.hpp"
#include "common/Math.hpp"

#define POLY_CORNER_COUNT 12

namespace mae
{
	static const player_color_t GREEN = {50,0,255,0};
	
	MarkerSensor::MarkerSensor(const RobotConfig& p_config)
		:graphics_(p_config.client->getClient(), p_config.graphicsIndex),
		 simulation_(p_config.simulation), stock_(p_config.stock), robotPose_(),
		 range_(p_config.markerSensorRange)
	{
		LOG(DEBUG) << "Connected GraphicsProxy: " << p_config.graphicsIndex << " (" << p_config.name << ")";
		drawPolygon();
	}
	
	void MarkerSensor::drawPolygon()
	{
		Vector2 points[POLY_CORNER_COUNT];
		player_point_2d_t playerPoints[POLY_CORNER_COUNT];
		createRegularPolygon(1, points, POLY_CORNER_COUNT);
		
		for(int i = 0; i < POLY_CORNER_COUNT; ++i) {
			playerPoints[i].px = points[i].x * range_;
			playerPoints[i].py = points[i].y * range_;
		}
		
		graphics_.Clear();
		graphics_.Color(GREEN);
		graphics_.DrawPolygon(playerPoints, POLY_CORNER_COUNT, false, GREEN);
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
			if(distance.lengthSQ() <= range_ * range_)
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
	
	double MarkerSensor::getRange() const
	{
		return range_;
	}

}
