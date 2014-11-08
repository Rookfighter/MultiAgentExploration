#ifndef MAE_ANT_STATE_HPP
#define MAE_ANT_STATE_HPP

#include <functional>
#include "simulation/ExplorationBot.hpp"
#include "simulation/MarkerStock.hpp"

namespace mae
{
	typedef void (*updateValueFunc)(Marker* p_current, Marker* p_next);
	typedef double(*calcValueFunc)(Marker* p_current, Marker* p_next);

	class AntStateProperties
	{
	public:
		ExplorationBot *robot;
		MarkerStock *stock;

		Marker *currentMarker;
		Marker *nextMarker;
		double angleToTurn;

		updateValueFunc updateValue;
		calcValueFunc calcValue;

		double obstacleAvoidDistance;
		double obstacleMarkerDistance;

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
