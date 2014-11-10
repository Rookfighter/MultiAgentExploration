#ifndef MAE_MOVING_TO_DIRECTION_HPP
#define MAE_MOVING_TO_DIRECTION_HPP

#include "algorithm/Wander.hpp"
#include "algorithm/ObstacleDetector.hpp"
#include "algorithm-rt/AntState.hpp"

namespace mae
{

	class MovingToDirection : public AntState
	{
	private:
		Wander wander_;
		ObstacleDetector obstacleDetector_;
		
		Pose lastPose_;
		double movedDistance_;
		double remainingToTurn_;
		bool reachedDirection_;
		
		void updateGeometry();
		bool reachedDirection();
		bool movedEnough();
		bool hasFrontObstacle();
		void move();
		void turn();
	public:
		MovingToDirection(const AntStateProperties &p_properties);
		~MovingToDirection();

		AntState* update();
	};

}

#endif
