#include <easylogging++.h>
#include "algorithm/SelectingTarget.hpp"
#include "algorithm/UpdatingValue.hpp"
#include "common/Math.hpp"
#include "common/Convert.hpp"

#define FRONT_IDX 0
#define BACK_IDX 2
#define LEFT_IDX 4
#define RIGHT_IDX 6

namespace mae
{
	static const double obstacleAngles[8] = {
		-M_PI / 6, M_PI / 6, // FRONT -30°, 30°
		5 * M_PI / 6, -5 * M_PI / 6, // BACK 150°, -150°
		2 * M_PI / 6, 4 * M_PI / 6, // LEFT 60°, 120°
		-4 * M_PI / 6, -2 * M_PI / 6 // RIGHT -120°, -60°
	};

	static const double markerAngles[8] = {
		-M_PI / 4, M_PI / 4, // FRONT -45°, 45°
		3 * M_PI / 4, -3 * M_PI / 4, // BACK 135°, -135°
		M_PI / 4, 3 * M_PI / 4, // LEFT 45°, 135°
		-3 * M_PI / 4, -M_PI / 4 // RIGHT -135°, -46°
	};


	SelectingTarget::SelectingTarget(const AntStateProperties &p_properties)
		: AntState(p_properties), obstacleThreshold_(p_properties.obstacleAvoidDistance)
	{
		LOG(DEBUG) << "New SelectingTarget state.";
		LOG(DEBUG) << "ObstacleThreshold: " << obstacleThreshold_;
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
		bool blockedFront = checkObstalce(obstacleAngles[FRONT_IDX], obstacleAngles[FRONT_IDX + 1]);
		bool blockedBack = checkObstalce(obstacleAngles[BACK_IDX], obstacleAngles[BACK_IDX + 1]);
		bool blockedLeft = checkObstalce(obstacleAngles[LEFT_IDX], obstacleAngles[LEFT_IDX + 1]);
		bool blockedRight = checkObstalce(obstacleAngles[RIGHT_IDX], obstacleAngles[RIGHT_IDX + 1]);

		LOG(DEBUG) << "-- Obstacles: F=" << boolToStr(blockedFront) << ", B=" <<  boolToStr(blockedBack) <<
		           ", L=" << boolToStr(blockedLeft) << ", R=" << boolToStr(blockedRight);
		// check if there is already a marker in that direction
		for(MarkerMeasurement measurement : markerInRange_) {
			if(!blockedFront)
				blockedFront = angleIsBetween(measurement.relativeDirection,
				                              markerAngles[FRONT_IDX],
				                              markerAngles[FRONT_IDX + 1]);

			if(!blockedBack)
				blockedBack = angleIsBetween(measurement.relativeDirection,
				                             markerAngles[BACK_IDX],
				                             markerAngles[BACK_IDX +1]);

			if(!blockedLeft)
				blockedLeft = angleIsBetween(measurement.relativeDirection,
				                             markerAngles[LEFT_IDX],
				                             markerAngles[LEFT_IDX + 1]);

			if(!blockedRight)
				blockedRight = angleIsBetween(measurement.relativeDirection,
				                              markerAngles[RIGHT_IDX],
				                              markerAngles[RIGHT_IDX + 1]);
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
