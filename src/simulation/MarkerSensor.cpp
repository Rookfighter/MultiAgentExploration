#include "simulation/MarkerSensor.hpp"
#include "common/Math.hpp"

namespace mae
{

	MarkerSensor::MarkerSensor(const RobotConfig& p_config)
		:simulation_(p_config.simulation), stock_(p_config.stock), robotPose_()
	{

	}

	MarkerSensor::~MarkerSensor()
	{

	}
	
	void MarkerSensor::setRobotPose(const Pose &p_pose)
	{
		robotPose_ = p_pose;
	}

	void MarkerSensor::getMarkerInRange(std::list<Marker*> &p_result)
	{
		Vector2 distance;
		
		for(Marker *marker : stock_->getMarker()) {
			distance = marker->getPose().position - robotPose_.position;
			if(distance.lengthSQ() <= marker->getRange() * marker->getRange())
				p_result.push_back(marker);
		}
	}

	Marker* MarkerSensor::getClosestMarker()
	{
		Marker *result = NULL;
		for(Marker *marker : stock_->getMarker()) {
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
		result = p_marker->getPose().position - robotPose_.position;
		return result;
	}

	double MarkerSensor::getAngleTo(Marker* p_marker)
	{
		// TODO - is this correct simulated -- distance has error, so is it ok
		// to use absolutePosition to determine angle offset?
		Vector2 distance = getDistanceTo(p_marker);
		double result = atan2(distance.y, distance.x) - normalizeRadian(robotPose_.yaw);
		result = normalizeRadian(result);
		return result;
	}


}
