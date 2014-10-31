#include <easylogging++.h>
#include "algorithm/SelectingTarget.hpp"
#include "algorithm/UpdatingValue.hpp"
#include "common/Math.hpp"
#include "common/Convert.hpp"

#define FRONT_ANGLE_BEGIN (-M_PI / 6) // -30°
#define FRONT_ANGLE_END (M_PI / 6) // 30°

#define BACK_ANGLE_BEGIN (5 * M_PI / 6) // 150°
#define BACK_ANGLE_END (-5 * M_PI / 6) // -150°

#define LEFT_ANGLE_BEGIN (2 * M_PI / 6) // 60
#define LEFT_ANGLE_END (4 * M_PI / 6) // 120°

#define RIGHT_ANGLE_BEGIN (-4 * M_PI / 6) //-120°
#define RIGHT_ANGLE_END (-2 * M_PI / 6) // -60°

namespace mae
{

	SelectingTarget::SelectingTarget(const AntStateProperties &p_properties)
		: AntState(p_properties), obstacleThreshold_(1.0)
	{
		LOG(DEBUG) << "New SelectingTarget state.";
	}

	SelectingTarget::~SelectingTarget()
	{
	}

	AntState* SelectingTarget::update()
	{
		if(properties_.nextMarker != NULL)
			properties_.nextMarker->setHighlighted(false);
		properties_.nextMarker = NULL;

		getMarkerInRange();
		if(!checkBlankSpace()) {
			getMarkerTarget();
			properties_.nextMarker->setHighlighted(true);
		}

		return new UpdatingValue(properties_);
	}

	void SelectingTarget::getMarkerInRange()
	{
		markerInRange_.clear();
		markerInRange_ = properties_.robot->getMarkerSensor().getMarkerInRange();

		// find the current marker in the list and remove it
		if(properties_.currentMarker != NULL) {
			std::vector<MarkerMeasurement>::iterator it;
			for(it = markerInRange_.begin(); it != markerInRange_.end(); ++it) {
				if(properties_.currentMarker->getID() == it->marker->getID())
					break;
			}
			markerInRange_.erase(it);
		}
	}

	bool SelectingTarget::checkBlankSpace()
	{
		// check if obstacle is the direction
		bool blockedFront = checkObstalce(FRONT_ANGLE_BEGIN, FRONT_ANGLE_END);
		bool blockedBack = checkObstalce(BACK_ANGLE_BEGIN, BACK_ANGLE_END);
		bool blockedLeft = checkObstalce(LEFT_ANGLE_BEGIN, LEFT_ANGLE_END);
		bool blockedRight = checkObstalce(RIGHT_ANGLE_BEGIN, RIGHT_ANGLE_END);

		LOG(DEBUG) << "-- Obstacles: F=" << boolToStr(blockedFront) << ", B=" <<  boolToStr(blockedBack) <<
		           ", L=" << boolToStr(blockedLeft) << ", R=" << boolToStr(blockedRight);
		// check if there is already a marker in that direction
		for(MarkerMeasurement measurement : markerInRange_) {
			if(!blockedFront)
				blockedFront = angleIsBetween(measurement.relativeDirection,
				                              FRONT_ANGLE_BEGIN,
				                              FRONT_ANGLE_END);

			if(!blockedBack)
				blockedBack = angleIsBetween(measurement.relativeDirection,
				                             BACK_ANGLE_BEGIN,
				                             BACK_ANGLE_END);

			if(!blockedLeft)
				blockedLeft = angleIsBetween(measurement.relativeDirection,
				                             LEFT_ANGLE_BEGIN,
				                             LEFT_ANGLE_END);

			if(!blockedRight)
				blockedRight = angleIsBetween(measurement.relativeDirection,
				                              RIGHT_ANGLE_BEGIN,
				                              RIGHT_ANGLE_END);
		}

		if(!blockedFront)
			properties_.angleToTurn = 0; // move forward
		else if(!blockedLeft)
			properties_.angleToTurn = M_PI / 2; // turn to left
		else if(!blockedRight)
			properties_.angleToTurn = -M_PI / 2; // turn right
		else if(!blockedBack)
			properties_.angleToTurn = M_PI; // turn back

		return  !blockedFront || !blockedBack || !blockedLeft || !blockedRight;
	}


	bool SelectingTarget::checkObstalce(const double p_beginAngle, const double p_endAngle)
	{
		double minDistance = 1e6;

		const RangerProperties& rangerProperties = properties_.robot->getRanger().getProperties();
		for(int i = 0; i < rangerProperties.getMeasurementCount(); ++i) {

			// check if sensor is in the angles we check
			if(!angleIsBetween(rangerProperties.getMeasurementOrigins()[i].yaw, p_beginAngle, p_endAngle))
				continue;

			double distance = properties_.robot->getRanger().getDistance(i);
			if(distance < minDistance)
				minDistance = distance;
		}

		return minDistance < obstacleThreshold_;
	}

	void SelectingTarget::getMarkerTarget()
	{
		assert(!markerInRange_.empty());

		for(MarkerMeasurement measurement : markerInRange_) {
			if(properties_.nextMarker == NULL ||
			        properties_.nextMarker->getValue() > measurement.marker->getValue())
				properties_.nextMarker = measurement.marker;
		}
	}


}
