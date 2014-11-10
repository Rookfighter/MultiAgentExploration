#include "algorithm/RandomWalk.hpp"

#define ANGLE_EPS ((M_PI / 180) * 5) // precision 5°
#define DISTANCE_EPS (0.10) // precsion 10cm
#define TURN_FACTOR (0.8)

#define TURN_GRANULARITY (4)
#define TURN_ANGLE ((2 * M_PI) / TURN_GRANULARITY)

namespace mae
{

	RandomWalk::RandomWalk(const AlgorithmConfig &p_config)
		:Algorithm(),
		robotController_(p_config.robot, p_config.obstacleAvoidDistance),
		wanderDistance_(p_config.robot->getMarkerSensor().getMaxRange())
	{
		robotController_.setAngleEps(ANGLE_EPS);
		robotController_.setDistanceEps(DISTANCE_EPS);
		robotController_.setTurnFactor(TURN_FACTOR);
		robotController_.wanderDistance(wanderDistance_);
	}

	RandomWalk::~RandomWalk()
	{
	}

	void RandomWalk::update()
	{
		if(robotController_.finished())
			chooseRandomDirection();
		else
			robotController_.update();
	}
	
	void RandomWalk::chooseRandomDirection()
	{
		double direction = random_.nextInt(TURN_GRANULARITY) * TURN_ANGLE;
		
		robotController_.turnBy(direction);
		robotController_.wanderDistance(wanderDistance_);
	}
	
}
