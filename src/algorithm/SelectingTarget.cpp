#include <easylogging++.h>
#include "algorithm/SelectingTarget.hpp"
#include "algorithm/UpdatingValue.hpp"
#include "common/Math.hpp"

#define MAX_ANGLE_DIFF (M_PI / 3) // 60°

namespace mae
{
	typedef struct {
		double angle;
		Marker* marker;
	} MarkerAngle;

	static bool compMarkerAngle(const MarkerAngle &a, const MarkerAngle &b)
	{
		return a.angle < b.angle;
	}

	SelectingTarget::SelectingTarget(const AntStateProperties &p_properties)
		: AntState(p_properties)
	{
		LOG(DEBUG) << "New SelectingTarget state.";
	}

	SelectingTarget::~SelectingTarget()
	{
	}

	AntState* SelectingTarget::update()
	{
		watch_.start();
		properties_.nextMarker = NULL;

		getMarkerInRange();
		if(!checkBlankSpace()) {
			getMarkerTarget();
		}
		
		watch_.stop();
		
		LOG(DEBUG) << "-- " << watch_.strMsec();
		
		return new UpdatingValue(properties_);
	}

	void SelectingTarget::getMarkerInRange()
	{
		markerInRange_.clear();
		properties_.robot->getMarkerSensor().getMarkerInRange(markerInRange_);

		if(properties_.currentMarker != NULL) {
			std::list<Marker*>::iterator it;
			for(it = markerInRange_.begin(); it != markerInRange_.end(); ++it) {
				if(properties_.currentMarker->getID() == (*it)->getID())
					break;
			}
			markerInRange_.erase(it);
		}
	}

	bool SelectingTarget::checkBlankSpace()
	{
		size_t markerCount = markerInRange_.size();

		if(markerCount < 2) {
			properties_.angleToTurn = 0;
			return true;
		} else if(markerCount >= 4) {
			return false;
		}
		int idx;
		
		// get angles to all markers in range
		// relative to robot yaw
		MarkerAngle markerAngleInRange[markerCount];
		idx = 0;
		for(Marker *marker : markerInRange_) {
			double angle = properties_.robot->getMarkerSensor().getAngleTo(marker);
			markerAngleInRange[idx].angle = angle;
			markerAngleInRange[idx].marker = marker;
			idx++;
		}
		
		// sort them so we have neighbouring markers next to each other
		std::sort(markerAngleInRange, markerAngleInRange + markerCount, compMarkerAngle);

		// calculate differences between neighbouring angles
		double angleDiffs[markerCount];
		for(int i = 0; i < markerCount; ++i) {
			int next = (i + 1) % markerCount;
			angleDiffs[i] = markerAngleInRange[next].angle - markerAngleInRange[i].angle;
		}
		
		// find the max difference
		int maxDiff = -1;
		for(int i = 0; i < markerCount; ++i) {
			if(maxDiff == -1 || fabs(angleDiffs[i]) > fabs(angleDiffs[maxDiff]))
				maxDiff = i;
		}
		
		if(fabs(angleDiffs[maxDiff]) >= MAX_ANGLE_DIFF) {
			properties_.angleToTurn = markerAngleInRange[maxDiff].angle + (angleDiffs[maxDiff] / 2);
			return true;
		} else {
			return false;
		}
	}

	void SelectingTarget::getMarkerTarget()
	{
		for(Marker *marker : markerInRange_) {
			if(properties_.nextMarker == NULL || properties_.nextMarker->getValue() > marker->getValue())
				properties_.nextMarker = marker;
		}
	}


}
