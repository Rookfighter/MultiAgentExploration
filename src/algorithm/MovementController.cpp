#include "algorithm/MovementController.hpp"
#include "utils/Math.hpp"

#define DEF_ANGLE_EPS ((M_PI / 180) * 5) // precision 5°

namespace mae
{

	MovementController::MovementController(ExplorationBot *p_robot,
	                                 const double p_obstacleAvoidDistance)
		:wander_(p_robot, p_obstacleAvoidDistance),
		 robot_(p_robot),
		 lastPose_(robot_->getMotor().getPose()),
		 angleToTurn_(0),
		 distanceToMove_(0),
		 angleEps_(DEF_ANGLE_EPS),
		 turnFactor_(1.0)
	{
	}

	MovementController::~MovementController()
	{
	}
	
	void MovementController::setAngleEps(const double p_angleEps)
	{
		angleEps_ = p_angleEps;
	}
	
	void MovementController::setTurnFactor(const double p_factor)
	{
		turnFactor_ = p_factor;
	}

	void MovementController::turnBy(const double p_angle)
	{
		angleToTurn_ = normalizeRadian(p_angle);
	}

	void MovementController::wanderDistance(const double p_distance)
	{
		distanceToMove_ = p_distance;
	}

	bool MovementController::reachedDistance() const
	{
		return distanceToMove_ <= 0;
	}

	bool MovementController::reachedDirection() const
	{
		return sameDouble(0, angleToTurn_, angleEps_);
	}
	
	bool MovementController::finished() const
	{
		return reachedDirection() && reachedDistance();
	}

	void MovementController::update()
	{
		updateGeometry();
		
		if(!reachedDirection())
			turn();
		else if(!reachedDistance())
			wander();
		else
			robot_->getMotor().stop();
	}
	
	void MovementController::updateGeometry()
	{
		Pose currentPose = robot_->getMotor().getPose();
		
		if(!reachedDirection())
			angleToTurn_ -= normalizeRadian(currentPose.yaw - lastPose_.yaw);
			
		if(!reachedDistance()){
			double distance = (currentPose.position - lastPose_.position).length();
			distanceToMove_ -= distance;
		}
		
		lastPose_ = currentPose;
	}
	
	void MovementController::turn()
	{
		double angularVelocity;
		if(angleToTurn_ < 0)
			angularVelocity = robot_->getMotor().getMinVelocity().angular * turnFactor_;
		else
			angularVelocity = robot_->getMotor().getMaxVelocity().angular * turnFactor_;

		robot_->getMotor().setVelocity(Velocity(0, angularVelocity));
	}
	
	void MovementController::wander()
	{
		wander_.update();
	}
}
