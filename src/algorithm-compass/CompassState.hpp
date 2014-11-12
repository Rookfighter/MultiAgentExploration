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
		CardinalDirection nextDirection;
        
        double obstacleAvoidDistance;
		double obstacleMarkerDistance;
		
		CompassStateProperties()
		:currentMarker(NULL), nextDirection(WEST) { }
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
