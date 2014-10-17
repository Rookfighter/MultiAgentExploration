#ifndef MAE_MOVE_TO_MARKER_HPP
#define MAE_MOVE_TO_MARKER_HPP

#include "control/ExplorationBot.hpp"

namespace mae
{

	class MoveToMarker
	{
	private:
		ExplorationBot &robot_;
		Velocity velocity_;
		double angleToTarget_;

		double frontStopDistance_;

		void turnToMarker(Marker *p_targetMarker);
		void moveTowardsMarker(Marker *p_targetMarker);
	public:
		MoveToMarker(ExplorationBot &p_robot,
		             const double p_frontStopDistance = 1.0);
		~MoveToMarker();

		void step(Marker *p_targetMarker);
	};

}

#endif
