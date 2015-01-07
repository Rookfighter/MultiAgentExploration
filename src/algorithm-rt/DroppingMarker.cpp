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
	    properties_.currentMarker = NULL;
	    if(!hasTooCloseMarker()) {
            Marker* dropped = properties_.robot->dropMarker();
            properties_.currentMarker = dropped;
            LOG(DEBUG) << "-- dropped marker id " << dropped->getID() << " (" << properties_.robot->getName() << ")";
	    }
        
		return new SelectingTarget(properties_);
	}

	bool DroppingMarker::hasTooCloseMarker() const
    {
	    MarkerMeasurement measurement = properties_.robot->getMarkerSensor().getClosestMarker();
        return (measurement.marker != NULL) && (measurement.relativeDistance.lengthSQ() <= properties_.markerTooCloseDistance * properties_.markerTooCloseDistance);
    }

}
