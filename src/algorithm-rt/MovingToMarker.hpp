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
        std::vector<MarkerMeasurement> markerInRange_;
		
		MarkerMeasurement targetMeasurement_;
		bool foundMarker_;
		
		int obstacleAvoidStep_;
		
		void updateTargetMeasurement();
		bool isFacingToTarget();
		bool reachedTarget();
        bool hasOtherMarkersInRange();
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
