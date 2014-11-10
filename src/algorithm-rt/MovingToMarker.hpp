#ifndef MAE_MOVING_TO_MARKER_HPP
#define MAE_MOVING_TO_MARKER_HPP

#include "algorithm/Wander.hpp"
#include "algorithm/ObstacleDetector.hpp"
#include "algorithm-rt/AntState.hpp"

namespace mae
{
	class MovingToMarker : public AntState
	{
	private:
		Wander wander_;
		ObstacleDetector obstacleDetector_;
		
		Pose lastPose_;
		double movedDistance_;
		MarkerMeasurement targetMeasurement_;
		bool foundMarker_;
		
		int obstacleAvoidStep_;
		
		void updateGeometry();
		void updateTargetMeasurement();
		bool reachedDirection();
		bool reachedTarget();
		bool movedEnough();
		void turnToMarker();
		void wander();
		bool isAvoidingObstacle();
		bool hasObstacleToTarget();
		
	public:
		MovingToMarker(const AntStateProperties &p_properties);
		~MovingToMarker();
		
		AntState* update();
	};

}

#endif
