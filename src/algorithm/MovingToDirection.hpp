#ifndef MAE_MOVING_TO_DIRECTION_HPP
#define MAE_MOVING_TO_DIRECTION_HPP

#include "algorithm/Wander.hpp"
#include "algorithm/AntState.hpp"

namespace mae
{

	class MovingToDirection : public AntState
	{
	private:
		Wander wander_;
		
		Pose lastPose_;
		double movedDistance_;
		double turnedAngle_;
		
		void updateGeometry();
		bool reachedTarget();
		bool reachedDirection();
		void move();
		void turn();
	public:
		MovingToDirection(const AntStateProperties &p_properties);
		~MovingToDirection();

		AntState* update();
	};

}

#endif
