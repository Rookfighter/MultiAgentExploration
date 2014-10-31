#ifndef MAE_MOVING_TO_MARKER_HPP
#define MAE_MOVING_TO_MARKER_HPP

#include "algorithm/Wander.hpp"
#include "algorithm/AntState.hpp"

namespace mae
{
	class MovingToMarker : public AntState
	{
	private:
		Wander wander_;
		
		Pose lastPose_;
		double movedDistance_;
		MarkerMeasurement targetMeasurement_;
		bool foundMarker_;
		bool reachedDirection_;
		
		void updateGeometry();
		void updateTargetMeasurement();
		bool reachedDirection();
		bool reachedTarget();
		bool movedEnough();
		void turnToMarker();
		void moveTowardsMarker();
	public:
		MovingToMarker(const AntStateProperties &p_properties);
		~MovingToMarker();
		
		AntState* update();
	};

}

#endif
