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
		std::vector<Marker*> markerInRange = properties_.robot->getMarkerInRange(properties_.stock->getInUse());
		if(markerInRange.empty())
			return new DroppingMarker(properties_);
		else {
			properties_.nextMarker = markerInRange[0];
			return new MovingToMarker(properties_);
		}
	}

}
