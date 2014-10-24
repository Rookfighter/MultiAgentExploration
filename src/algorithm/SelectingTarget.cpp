#include <easylogging++.h>
#include "algorithm/SelectingTarget.hpp"
#include "algorithm/UpdatingValue.hpp"
#include "common/Math.hpp"

#define MAX_ANGLE_DIFF (M_PI / 2) // 90°

namespace mae
{
	typedef struct {
		double angle;
		int index;
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
		properties_.nextMarker = NULL;

		getMarkerInRange();
		if(!checkBlankSpace()) {
			getMarkerTarget();
		}

		return new UpdatingValue(properties_);
	}

	void SelectingTarget::getMarkerInRange()
	{
		markerInRange_.clear();
		std::vector<Marker*> markerInRangeTmp = properties_.robot->getMarkerSensor().getMarkerInRange();

		if(properties_.currentMarker == NULL) {
			markerInRange_ = markerInRangeTmp;
		} else {
			markerInRange_.reserve(markerInRangeTmp.size() - 1);
			for(Marker *marker : markerInRangeTmp) {
				if(properties_.currentMarker->getID() != marker->getID())
					markerInRange_.push_back(marker);
			}
		}
	}

	bool SelectingTarget::checkBlankSpace()
	{
		size_t markerCount = markerInRange_.size();

		if(markerCount == 0) {
			properties_.angleToTurn = 0;
			return true;
		} else if(markerCount == 1) {
			properties_.angleToTurn = properties_.robot->getMarkerSensor().getAngleTo(markerInRange_[0])
			                          + MAX_ANGLE_DIFF;
			return true;
		}

		// get angles to all markers in range
		// relative to robot yaw
		MarkerAngle markerAngleInRange[markerCount];
		for(int i = 0; i < markerCount; ++i) {
			double angle = properties_.robot->getMarkerSensor().getAngleTo(markerInRange_[i]);
			markerAngleInRange[i].angle = angle;
			markerAngleInRange[i].index = i;
		}
		// sort them so we have neighbouring markers next to each other
		std::sort(markerAngleInRange, markerAngleInRange + markerCount, compMarkerAngle);

		// calculate differences between neighbouring angles
		double angleDiffs[markerCount];
		for(int i = 0; i < markerCount; ++i) {
			int next = (i + 1) % markerCount;
			angleDiffs[i] = markerAngleInRange[i].angle - markerAngleInRange[next].angle;
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
