#include <easylogging++.h>
#include "algorithm-rt/SelectingTarget.hpp"
#include "algorithm-rt/UpdatingValue.hpp"
#include "utils/Math.hpp"
#include "utils/Convert.hpp"
#include "utils/Random.hpp"

#define FRONT_IDX 0
#define BACK_IDX 2
#define LEFT_IDX 4
#define RIGHT_IDX 6

#define MARKER_OBSTACLE_FOV (M_PI / 4) // 45°

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
		: AntState(p_properties), obstacleMarkerDistance_(p_properties.obstacleMarkerDistance)
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
		properties_.angleToTurn = 0;

		getMarkerInRange();
		// check if there is any direction without marker to take
		if(!checkBlankSpace()) {
			LOG(DEBUG) << "-- no blank found";
			// check if we can find any marker
			if(findNextMarker()) {
				properties_.nextMarker->setHighlighted(true);
				LOG(DEBUG) << "-- found next marker";
			} else
				LOG(DEBUG) << "-- no fitting next marker found";
		} else
			LOG(DEBUG) << "-- found blank";

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
		bool blockedFront = checkObstacle(obstacleAngles[FRONT_IDX],
		                                  obstacleAngles[FRONT_IDX + 1],
		                                  obstacleMarkerDistance_);
		bool blockedBack = checkObstacle(obstacleAngles[BACK_IDX],
		                                 obstacleAngles[BACK_IDX + 1],
		                                 obstacleMarkerDistance_);
		bool blockedLeft = checkObstacle(obstacleAngles[LEFT_IDX],
		                                 obstacleAngles[LEFT_IDX + 1],
		                                 obstacleMarkerDistance_);
		bool blockedRight = checkObstacle(obstacleAngles[RIGHT_IDX],
		                                  obstacleAngles[RIGHT_IDX + 1],
		                                  obstacleMarkerDistance_);

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


	bool SelectingTarget::checkObstacle(const double p_beginAngle,
	                                    const double p_endAngle,
	                                    const double p_distance)
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

		return minDistance < p_distance;
	}

	bool SelectingTarget::findNextMarker()
	{
		assert(properties_.robot->getMarkerSensor().getMaxRange() <
		       properties_.robot->getRanger().getProperties().getMaxRange());

		if(markerInRange_.empty())
			return false;

		std::vector<MarkerMeasurement> possibleTargets = getNonObstructedMarker();
		if(possibleTargets.empty())
			return false;
		Random rand;
		int idx = rand.nextInt(possibleTargets.size());
		properties_.nextMarker = possibleTargets[idx].marker;

		return true;
	}

	std::vector<MarkerMeasurement> SelectingTarget::getNonObstructedMarker()
	{
		std::vector<MarkerMeasurement> result;
		int minValue = getMinNonObstructedMarkerValue();

		if(minValue == -1)
			return result;

		for(int i = 0; i < markerInRange_.size(); ++i) {
			if(markerInRange_[i].marker->getValue() == minValue &&
			        !markerIsObstructed(markerInRange_[i])) {
				result.push_back(markerInRange_[i]);
			}
		}

		return result;
	}

	int SelectingTarget::getMinNonObstructedMarkerValue()
	{
		bool foundMarker = false;
		int nextIdx;
		bool alreadyChecked[markerInRange_.size()];
		int checkedCount = 0;

		for(int i = 0; i < markerInRange_.size(); ++i)
			alreadyChecked[i] = false;

		while(!foundMarker && checkedCount < markerInRange_.size()) {
			nextIdx = -1;
			// search marker with lowest value
			for(int i = 0; i < markerInRange_.size(); ++i) {
				bool hasLowestValue = !alreadyChecked[i] &&
				                      (nextIdx == -1 ||
				                       markerInRange_[nextIdx].marker->getValue() > markerInRange_[i].marker->getValue());
				if(hasLowestValue)
					nextIdx = i;
			}
			// exclude this marker from future iterations
			alreadyChecked[nextIdx] = true;
			checkedCount++;

			LOG(DEBUG) << "-- checking marker: " <<
			           radianToDegree(markerInRange_[nextIdx].relativeDirection) << "° " <<
			           markerInRange_[nextIdx].relativeDistance.length() << "m";
			// check if the way to the marker is blocked by an obstacle
			foundMarker = !markerIsObstructed(markerInRange_[nextIdx]);
		}

		if(foundMarker)
			return markerInRange_[nextIdx].marker->getValue();
		else
			return -1;
	}

	bool SelectingTarget::markerIsObstructed(const MarkerMeasurement &p_markerMeasurement)
	{
		return checkObstacle(p_markerMeasurement.relativeDirection - (MARKER_OBSTACLE_FOV / 2),
		                     p_markerMeasurement.relativeDirection + (MARKER_OBSTACLE_FOV / 2),
		                     p_markerMeasurement.relativeDistance.length());
	}


}
