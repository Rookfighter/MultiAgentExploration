#include "algorithm/MovementController.hpp"
#include "utils/Math.hpp"

#define DEF_ANGLE_EPS ((M_PI / 180) * 5) // precision 5°

namespace mae
{
    /*TODO if robot stuck bug also appears in MovingToDirection State
      only source of bug can be MovementController or Wander. */

    MovementController::MovementController(ExplorationBot *p_robot,
                                           const double p_obstacleStopDistance,
                                           const double p_obstacleAvoidDistance,
                                           const double p_collisionResolveDistance)
        :wander_(p_robot,
                 p_obstacleStopDistance,
                 p_obstacleAvoidDistance),
        collisionResolver_(p_robot,
                           p_collisionResolveDistance,
                           p_obstacleStopDistance),
        robot_(p_robot),
        lastPose_(robot_->getMotor().getPose()),
        angleToTurn_(0),
        distanceToMove_(0),
        minDistanceToMove_(0),
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

    void MovementController::wanderMinDistance(const double p_distance)
    {
        minDistanceToMove_ = p_distance;
    }

    void MovementController::stopWandering()
    {
        wanderDistance(0);
        wanderMinDistance(0);
    }

    void MovementController::stopTurning()
    {
        turnBy(0);
    }

    bool MovementController::reachedDistance() const
    {
        return distanceToMove_ <= 0;
    }

    bool MovementController::reachedMinDistance() const
    {
        return minDistanceToMove_ <= 0;
    }

    bool MovementController::reachedDirection() const
    {
        return sameDouble(0, angleToTurn_, angleEps_);
    }

    bool MovementController::finished() const
    {
        return reachedDirection() && reachedDistance();
    }

    bool MovementController::isAvoidingObstacle() const
    {
        //TODO maybe add '&& collisionResolver_.finished()'
        //or else may return true when we are resolving a crash
        return wander_.isAvoidingObstacle();
    }

    void MovementController::update()
    {
        updateGeometry();
        
        if(hasCollision())
            resolveCollision();
        else {
            if(!reachedDirection())
                turn();
            else if(!reachedDistance())
                wander();
            else
                robot_->getMotor().stop();
        }
    }

    void MovementController::updateGeometry()
    {
        Pose currentPose = robot_->getMotor().getPose();
        
        if(!reachedDirection())
            angleToTurn_ -= normalizeRadian(currentPose.yaw - lastPose_.yaw);

        double distance = (currentPose.position - lastPose_.position).length();
        if(!reachedDistance()) {
            distanceToMove_ -= distance;
        }

        if(!reachedMinDistance()) {
            minDistanceToMove_ -= distance;
        }

        lastPose_ = currentPose;
    }

    bool MovementController::hasCollision() const
    {
        return robot_->collidedWithObstacle() || !collisionResolver_.finished();
    }

    void MovementController::resolveCollision()
    {
        // still have collision, although we finished
        if(collisionResolver_.finished())
            collisionResolver_.resolveCollision();
        collisionResolver_.update();

        // refresh pose of movement controller, so backwards distance is not counted
        if(collisionResolver_.finished())
            refreshPose();
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

    void MovementController::refreshPose()
    {
        lastPose_ = robot_->getMotor().getPose();
    }
}
