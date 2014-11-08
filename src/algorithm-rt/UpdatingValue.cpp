#include <easylogging++.h>
#include "algorithm-rt/UpdatingValue.hpp"
#include "algorithm-rt/MovingToDirection.hpp"
#include "algorithm-rt/MovingToMarker.hpp"

namespace mae
{

	UpdatingValue::UpdatingValue(const AntStateProperties &p_properties)
		:AntState(p_properties)
	{
		LOG(DEBUG) << "Changed to UpdatingValue state";
	}

	UpdatingValue::~UpdatingValue()
	{
	}

	AntState* UpdatingValue::update()
	{
		if(properties_.currentMarker != NULL) {
			LOG(DEBUG) << "-- cannot update value, no currentMarker";
			properties_.updateValue(properties_.currentMarker, properties_.nextMarker);
		}
		
		if(properties_.nextMarker == NULL)
			return new MovingToDirection(properties_);
		else
			return new MovingToMarker(properties_);
	}

}
