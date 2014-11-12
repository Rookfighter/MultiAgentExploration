#include "algorithm-rt/AntState.hpp"

namespace mae
{

	AntStateProperties::AntStateProperties()
		:robot(NULL),
		 currentMarker(NULL), nextMarker(NULL), angleToTurn(0),
		 obstacleAvoidDistance(1.0), obstacleMarkerDistance(1.0)
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
