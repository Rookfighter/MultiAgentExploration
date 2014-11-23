#include <easylogging++.h>
#include "algorithm-rt/UpdatingValue.hpp"
#include "algorithm-rt/MovingToDirection.hpp"
#include "algorithm-rt/MovingToMarker.hpp"

namespace mae
{

    UpdatingValue::UpdatingValue(const AntStateProperties &p_properties)
        :AntState(p_properties)
    {
        LOG(DEBUG) << "Changed to UpdatingValue state (" << properties_.robot->getName() << ")";
    }

    UpdatingValue::~UpdatingValue()
    {
    }

    AntState* UpdatingValue::update()
    {
        if(properties_.currentMarker != NULL) {
            LOG(DEBUG) << "-- updating marker " << properties_.currentMarker->getID() << " (" << properties_.robot->getName() << ")";
            LOG(DEBUG) << "-- old value is " << properties_.currentMarker->getValue() << " (" << properties_.robot->getName() << ")";
            properties_.updateValue(properties_.currentMarker, properties_.nextMarker);
            LOG(DEBUG) << "-- new value is " << properties_.currentMarker->getValue() << " (" << properties_.robot->getName() << ")";
        } else
            LOG(DEBUG) << "-- cannot update value, no currentMarker (" << properties_.robot->getName() << ")";

        if(properties_.nextMarker == NULL)
            return new MovingToDirection(properties_);
        else
            return new MovingToMarker(properties_);
    }

}
