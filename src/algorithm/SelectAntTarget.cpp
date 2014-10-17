#include "algorithm/SelectAntTarget.hpp"

#define MAX_ANGLE_DIFF (M_PI / 6) // 30°

namespace mae
{
	#define DIRECTIONS_COUNT 4
	static const double directions[] = {M_PI, M_PI / 2, 0, -M_PI / 2 };
	
	SelectAntTarget::SelectAntTarget(ExplorationBot &p_robot)
	:robot_(p_robot)
	{
		
	}
	
	SelectAntTarget::~SelectAntTarget()
	{
		
	}
	
	AntTarget SelectAntTarget::execute(const std::vector<Marker*> &p_availableMarker)
	{
		getMarkerInRange(p_availableMarker);
		
		for(int i = 0; i < DIRECTIONS_COUNT; ++i)
				directionMarked_[i] = false;
		
		for(Marker *marker : markerInRange_) {
			double angle = robot_.getAngleTo(marker);
			for(int i = 0; i < DIRECTIONS_COUNT; ++i) {
				if(!directionMarked_[i] && fabs(angle - directions[i]) < MAX_ANGLE_DIFF)
					directionMarked_[i] = true;
			}
		}
	}
	
	void SelectAntTarget::getMarkerInRange(const std::vector<Marker*> &p_availableMarker)
	{
		markerInRange_.reserve(p_availableMarker.size());
		for(Marker *marker : p_availableMarker) {
			Vector2 distance = robot_.getDistanceTo(marker);
			if(distance.lengthSQ() <= marker->range * marker->range)
				markerInRange_.push_back(marker);
		}
	}


}
