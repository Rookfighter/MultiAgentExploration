#ifndef MAE_ANT_STATE_HPP
#define MAE_ANT_STATE_HPP

#include <functional>
#include "simulation/ExplorationBot.hpp"
#include "simulation/MarkerStock.hpp"

namespace mae
{
	class AntStateProperties
	{
	public:
		ExplorationBot *robot;

		Marker *currentMarker;
		Marker *nextMarker;
		double angleToTurn;
		
		std::function<void(Marker*,Marker*)> updateValue;
		std::function<double(Marker*,Marker*)> calcValue;
        
        double obstacleStopDistance;
		double obstacleAvoidDistance;
		double obstacleMarkerDistance;
        double markerDeployDistance;
        double collisionResolveDistance;
        double markerTooCloseDistance;

		AntStateProperties();
		~AntStateProperties();
	};

	class AntState
	{
	protected:
		AntStateProperties properties_;
	public:
		AntState(const AntStateProperties &p_properties);
		virtual ~AntState();

		virtual AntState* update() = 0;
	};

}

#endif
