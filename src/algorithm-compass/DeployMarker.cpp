#include <easylogging++.h>
#include "algorithm-compass/DeployMarker.hpp"
#include "algorithm-compass/AtMarker.hpp"

namespace mae
{

	DeployMarker::DeployMarker(const CompassStateProperties &p_properties)
	:CompassState(p_properties)
	{
        LOG(DEBUG) << "Changed to DeployMarker state";
	}

	DeployMarker::~DeployMarker()
	{
	}

    CompassState* DeployMarker::update()
    {
        properties_.currentMarker = properties_.robot->dropMarker();
        
        if(properties_.lastMarker != NULL) {
            CardinalDirection originDirection = getOppositeDirection(properties_.robot->getCompass().getFacingDirection());
            properties_.currentMarker->exploreDirection(originDirection);
        }
        
        return new AtMarker(properties_);
    }
}
