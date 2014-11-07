#include <easylogging++.h>
#include "algorithm-rt/UpdatingValue.hpp"
#include "algorithm-rt/MovingToDirection.hpp"
#include "algorithm-rt/MovingToMarker.hpp"

namespace mae
{

	UpdatingValue::UpdatingValue(const AntStateProperties &p_properties)
		:AntState(p_properties)
	{
		LOG(DEBUG) << "New UpdatingValue state.";
	}

	UpdatingValue::~UpdatingValue()
	{
	}

	AntState* UpdatingValue::update()
	{
		assert(properties_.currentMarker != NULL);
		
		properties_.updateValue(properties_.currentMarker, properties_.nextMarker);
		
		if(properties_.nextMarker == NULL)
			return new MovingToDirection(properties_);
		else
			return new MovingToMarker(properties_);
	}

}
