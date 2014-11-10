#ifndef MAE_MOVING_TO_MARKER_HPP
#define MAE_MOVING_TO_MARKER_HPP

#include "algorithm/MovementController.hpp"
#include "algorithm/ObstacleDetector.hpp"
#include "algorithm-rt/AntState.hpp"

namespace mae
{
	class MovingToMarker : public AntState
	{
	private:
		MovementController movementController_;
		ObstacleDetector obstacleDetector_;
		
		MarkerMeasurement targetMeasurement_;
		bool foundMarker_;
		
		int obstacleAvoidStep_;
		
		void updateTargetMeasurement();
		bool isFacingToTarget();
		bool reachedTarget();
		void move();
		void turnToTarget();
		bool isAvoidingObstacle();
		bool hasObstacleToTarget();
	public:
		MovingToMarker(const AntStateProperties &p_properties);
		~MovingToMarker();
		
		AntState* update();
	};

}

#endif
