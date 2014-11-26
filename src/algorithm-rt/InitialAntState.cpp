#include <easylogging++.h>
#include "algorithm-rt/InitialAntState.hpp"
#include "algorithm-rt/DroppingMarker.hpp"
#include "algorithm-rt/SelectingTarget.hpp"

namespace mae
{

	InitialAntState::InitialAntState(const AntStateProperties &p_properties)
	:AntState(p_properties)
	{
		LOG(DEBUG) << "Changed to InitialAntState state";
	}

	InitialAntState::~InitialAntState()
	{
	}

	AntState* InitialAntState::update()
	{
		std::vector<MarkerMeasurement> markerInRange;
		markerInRange = properties_.robot->getMarkerSensor().getMarkerInRange();
		if(markerInRange.empty()) {
			return new DroppingMarker(properties_);
		} else {
			return new SelectingTarget(properties_);
		}
	}

}
