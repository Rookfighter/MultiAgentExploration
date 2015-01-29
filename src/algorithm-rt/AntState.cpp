#include "algorithm-rt/AntState.hpp"

namespace mae
{

	AntStateProperties::AntStateProperties()
		:robot(NULL),
		 currentMarker(NULL),
         nextMarker(NULL),
         obstacleStopDistance(-1),
		 obstacleAvoidDistance(-1),
         obstacleMarkerDistance(-1),
         markerDeployDistance(-1),
         collisionResolveDistance(-1),
         markerTooCloseDistance(-1)
	{

	}

	AntStateProperties::~AntStateProperties()
	{

	}

	AntState::AntState(const AntStateProperties &p_properties)
		:properties_(p_properties)
	{
	}

	AntState::~AntState()
	{
	}


}
