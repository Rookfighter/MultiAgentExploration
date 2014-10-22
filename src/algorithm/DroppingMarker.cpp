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
		Marker* dropped = properties_.robot->dropMarker();
		properties_.currentMarker = dropped;
		
		return new SelectingTarget(properties_);
	}

}
