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
		if(properties_.currentMarker != NULL)
			properties_.updateValue(properties_.currentMarker, properties_.nextMarker);
		else
			LOG(DEBUG) << "-- cannot update value, no currentMarker";
		
		if(properties_.nextMarker == NULL)
			return new MovingToDirection(properties_);
		else
			return new MovingToMarker(properties_);
	}

}
