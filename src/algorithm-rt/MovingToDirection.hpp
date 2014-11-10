#ifndef MAE_MOVING_TO_DIRECTION_HPP
#define MAE_MOVING_TO_DIRECTION_HPP

#include "algorithm/MovementController.hpp"
#include "algorithm/ObstacleDetector.hpp"
#include "algorithm-rt/AntState.hpp"

namespace mae
{

	class MovingToDirection : public AntState
	{
	private:
		MovementController movementController_;
		ObstacleDetector obstacleDetector_;
		
		bool hasFrontObstacle();
	public:
		MovingToDirection(const AntStateProperties &p_properties);
		~MovingToDirection();

		AntState* update();
	};

}

#endif
