#include <easylogging++.h>
#include "algorithm-compass/InitialCompassState.hpp"
#include "algorithm-compass/DeployMarker.hpp"
#include "algorithm-compass/AtMarker.hpp"

namespace mae
{

    InitialCompassState::InitialCompassState(const CompassStateProperties &p_properties)
        :CompassState(p_properties)
    {
        LOG(DEBUG) << "Changed to InitialCompassState state";
    }

    InitialCompassState::~InitialCompassState()
    {
    }

    CompassState* InitialCompassState::update()
    {
        std::vector<MarkerMeasurement> markerInRange = properties_.robot->getMarkerSensor().getMarkerInRange();
        
        if(markerInRange.empty()) {
            return new DeployMarker(properties_);
        } else {
            int idx = random_.nextInt(markerInRange.size());
            properties_.currentMarker = markerInRange[idx].marker;
            return new AtMarker(properties_);
        }
            
    }
}
