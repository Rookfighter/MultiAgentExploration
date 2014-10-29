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
		std::list<Marker*> markerInRange;
		properties_.robot->getMarkerSensor().getMarkerInRange(markerInRange);
		if(markerInRange.empty())
			return new DroppingMarker(properties_);
		else {
			properties_.nextMarker = properties_.robot->getMarkerSensor().getClosestMarker();
			return new MovingToMarker(properties_);
		}
	}

}
