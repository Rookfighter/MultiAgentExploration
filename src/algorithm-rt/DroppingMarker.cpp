#include <easylogging++.h>
#include "algorithm-rt/DroppingMarker.hpp"
#include "algorithm-rt/SelectingTarget.hpp"

namespace mae
{

	DroppingMarker::DroppingMarker(const AntStateProperties &p_properties)
	:AntState(p_properties)
	{
		LOG(DEBUG) << "Changed to DroppingMarker state (" << p_properties.robot->getName() << ")";
	}

	DroppingMarker::~DroppingMarker()
	{
	}

	AntState* DroppingMarker::update()
	{
		Marker* dropped = properties_.robot->dropMarker();
		properties_.currentMarker = dropped;
        LOG(DEBUG) << "-- dropped marker id " << dropped->getID() << " (" << properties_.robot->getName() << ")";
        
		return new SelectingTarget(properties_);
	}

}
