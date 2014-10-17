#include "algorithm/MoveToMarker.hpp"
#include "common/Math.hpp"

#define MAX_ANGLE_DIFF (M_PI / 18) // 10°

#define TURN_FACTOR 0.8
#define MOVE_FACTOR 1.0

namespace mae
{

	MoveToMarker::MoveToMarker(ExplorationBot &p_robot,
	                           const double p_frontStopDistance)
		:robot_(p_robot), frontStopDistance_(p_frontStopDistance)
	{
	}

	MoveToMarker::~MoveToMarker()
	{
	}

	void MoveToMarker::step(Marker *p_targetMarker)
	{
		angleToTarget_ = robot_.getAngleTo(p_targetMarker);

		if(fabs(angleToTarget_) > MAX_ANGLE_DIFF)
			turnToMarker(p_targetMarker);
		else
			moveTowardsMarker(p_targetMarker);

		robot_.setVelocity(velocity_);
	}

	void MoveToMarker::turnToMarker(Marker *p_targetMarker)
	{
		if(angleToTarget_ < 0)
			velocity_.angular = TURN_FACTOR * robot_.getMinVelocity().angular;
		else
			velocity_.angular = TURN_FACTOR * robot_.getMaxVelocity().angular;

		velocity_.linear = 0;

	}

	void MoveToMarker::moveTowardsMarker(Marker *p_targetMarker)
	{
		velocity_.linear = MOVE_FACTOR * robot_.getMaxVelocity().linear;
		velocity_.angular = 0;
	}
}
