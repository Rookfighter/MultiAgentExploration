#include <easylogging++.h>
#include "algorithm-compass/DeployMarker.hpp"
#include "algorithm-compass/AtMarker.hpp"

namespace mae
{

	DeployMarker::DeployMarker(const CompassStateProperties &p_properties)
	:CompassState(p_properties)
	{
        LOG(DEBUG) << "Changed to DeployMarker state (" << properties_.robot->getName() << ")";
	}

	DeployMarker::~DeployMarker()
	{
	}

    CompassState* DeployMarker::update()
    {
        properties_.currentMarker = properties_.robot->dropMarker();
        LOG(DEBUG) << "-- dropped marker " << properties_.currentMarker->getID() << " (" << properties_.robot->getName() << ")";
        
        if(properties_.lastMarker != NULL) {
            CardinalDirection originDirection = getOppositeDirection(properties_.robot->getCompass().getFacingDirection());
            properties_.currentMarker->exploreDirection(originDirection);
            LOG(DEBUG) << "-- came from marker " << properties_.lastMarker->getID() << " " << getDirectionStr(originDirection) << " (" << properties_.robot->getName() << ")";
        }
        
        return new AtMarker(properties_);
    }
}
