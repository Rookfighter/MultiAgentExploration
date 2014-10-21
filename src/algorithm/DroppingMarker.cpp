#include <easylogging++.h>
#include "algorithm/DroppingMarker.hpp"
#include "algorithm/SelectingTarget.hpp"

namespace mae
{

	DroppingMarker::DroppingMarker(const AntStateProperties &p_properties)
	:AntState(p_properties)
	{
		LOG(DEBUG) << "New DroppingMarker state.";
	}

	DroppingMarker::~DroppingMarker()
	{
	}

	AntState* DroppingMarker::update()
	{
		Marker *toDrop = properties_.stock->acquireMarker();
		
		properties_.robot->placeMarker(toDrop);
		properties_.currentMarker = toDrop;
		
		return new SelectingTarget(properties_);
	}

}
