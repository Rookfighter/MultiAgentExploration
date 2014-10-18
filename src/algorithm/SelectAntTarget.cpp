#include <algorithm>
#include "algorithm/SelectAntTarget.hpp"

#define MAX_ANGLE_DIFF (M_PI / 2) // 90°

namespace mae
{
	typedef struct {
		double angle;
		int index;
	} MarkerAngle;

	bool compMarkerAngle(const MarkerAngle &a, const MarkerAngle &b)
	{
		return a.angle < b.angle;
	}

	SelectAntTarget::SelectAntTarget(ExplorationBot &p_robot)
		:robot_(p_robot)
	{

	}

	SelectAntTarget::~SelectAntTarget()
	{

	}

	AntTarget SelectAntTarget::execute(Marker* p_currentMarker,
	                                   const std::vector<Marker*> &p_availableMarker)
	{
		getMarkerInRange(p_currentMarker, p_availableMarker);
		if(!checkBlankSpace()) {
			getMarkerTarget();
		}
		
		return target_;
		
	}

	void SelectAntTarget::getMarkerInRange(Marker* p_currentMarker,
	                                       const std::vector<Marker*> &p_availableMarker)
	{
		markerInRange_.reserve(p_availableMarker.size());
		for(Marker *marker : p_availableMarker) {
			Vector2 distance = robot_.getDistanceTo(marker);
			if((p_currentMarker == NULL || p_currentMarker != marker) &&
			        distance.lengthSQ() <= marker->range * marker->range)
				markerInRange_.push_back(marker);
		}
	}
	
	bool SelectAntTarget::checkBlankSpace()
	{
		size_t markerCount = markerInRange_.size();
		
		if(markerCount == 0) {
			target_.angleToTurn = 0;
			return true;
		} else if( markerCount == 1) {
			target_.angleToTurn = robot_.getAngleTo(markerInRange_[0]) + MAX_ANGLE_DIFF;
			return true;
		}
		
		// get angles to all markers in range
		// relative to robot yaw
		MarkerAngle markerAngleInRange[markerCount];
		for(int i = 0; i < markerCount; ++i) {
			double angle = robot_.getAngleTo(markerInRange_[i]);
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
			target_.angleToTurn = markerAngleInRange[maxDiff].angle + (angleDiffs[maxDiff] / 2);
			return true;
		} else {
			return false;
		}
	}
	
	void SelectAntTarget::getMarkerTarget()
	{
		target_.marker = NULL;
		for(Marker *marker : markerInRange_) {
			if(target_.marker == NULL || target_.marker->value > marker->value)
				target_.marker = marker;
		}
			
	}


}
