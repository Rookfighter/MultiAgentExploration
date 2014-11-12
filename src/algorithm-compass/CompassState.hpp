#ifndef MAE_COMPASS_STATE_HPP
#define MAE_COMPASS_STATE_HPP

#include "simulation/ExplorationBot.hpp"

namespace mae
{
	class CompassStateProperties
	{
	public:
        ExplorationBot *robot;
		Marker *currentMarker;
        
        double obstacleAvoidDistance;
        double markerDeployDistance;
		
		CompassStateProperties()
		:currentMarker(NULL) { }
	};

	class CompassState
	{
	protected:
		CompassStateProperties properties_;
	public:
		CompassState(const CompassStateProperties &p_properties)
			:properties_(p_properties)
		{ }
		
		virtual ~CompassState() { }

		virtual CompassState* update() = 0;

	};

}

#endif
