#include <easylogging++.h>
#include "algorithm/InitialAntState.hpp"
#include "algorithm/DroppingMarker.hpp"
#include "algorithm/MovingToMarker.hpp"

namespace mae
{

	InitialAntState::InitialAntState(const AntStateProperties &p_properties)
	:AntState(p_properties)
	{
		LOG(DEBUG) << "New InitialAntState.";
	}

	InitialAntState::~InitialAntState()
	{
	}

	AntState* InitialAntState::update()
	{
		std::vector<MarkerMeasurement> markerInRange;
		markerInRange = properties_.robot->getMarkerSensor().getMarkerInRange();
		if(markerInRange.empty())
			return new DroppingMarker(properties_);
		else {
			properties_.nextMarker = properties_.robot->getMarkerSensor().getClosestMarker().marker;
			return new MovingToMarker(properties_);
		}
	}

}
