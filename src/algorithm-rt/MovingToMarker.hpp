#ifndef MAE_MOVING_TO_MARKER_HPP
#define MAE_MOVING_TO_MARKER_HPP

#include "algorithm/Wander.hpp"
#include "algorithm-rt/AntState.hpp"

namespace mae
{
	class MovingToMarker : public AntState
	{
	private:
		enum State {MOVING, AVOIDING, AFTER_AVOIDING};
		
		Wander wander_;
		
		Pose lastPose_;
		double movedDistance_;
		MarkerMeasurement targetMeasurement_;
		bool foundMarker_;
		
		State state_;
		int obstacleAvoidStep_;
		static const int OBSTACLE_AVOID_MAX_STEP = 5;
		
		void updateGeometry();
		void updateTargetMeasurement();
		bool reachedDirection();
		bool reachedTarget();
		bool movedEnough();
		void turnToMarker();
		void moveTowardsMarker();
		bool isAvoidingObstacle();
		
		void onAvoidBegin();
		void onAvoidEnd();
	public:
		MovingToMarker(const AntStateProperties &p_properties);
		~MovingToMarker();
		
		AntState* update();
	};

}

#endif
