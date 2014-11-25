#include "algorithm/CollisionResolver.hpp"
#include "utils/Math.hpp"

#define BACK_OBSTACLE_FOV (M_PI / 3) // 60°

namespace mae
{

    CollisionResolver::CollisionResolver(ExplorationBot *p_robot,
                                         const double p_collisionResolveDistance,
                                         const double p_backStopDistance)
        :robot_(p_robot),
         obstacleDetector_(p_robot),
         lastPose_(robot_->getMotor().getPose()),
         collisionResolveDistance_(p_collisionResolveDistance),
         backStopDistance_(p_backStopDistance),
         distanceToMove_(0)
    {

    }

    CollisionResolver::~CollisionResolver()
    {

    }

    void CollisionResolver::resolveCollision()
    {
        distanceToMove_ = collisionResolveDistance_;
        lastPose_ = robot_->getMotor().getPose();
    }

    bool CollisionResolver::finished()
    {
        return distanceToMove_ <= 0;
    }

    void CollisionResolver::update()
    {
        updateGeometry();

        if(!finished())
            moveBackwards();
        else
            robot_->getMotor().stop();
    }

    void CollisionResolver::updateGeometry()
    {
        Pose currentPose = robot_->getMotor().getPose();

        distanceToMove_ -= (currentPose.position - lastPose_.position).length();
        lastPose_ = currentPose;
        
        if(hasBackObstacle())
            distanceToMove_ = 0;
    }

    void CollisionResolver::moveBackwards()
    {
        Velocity velocity = robot_->getMotor().getMinVelocity();
        velocity.angular = 0;
        robot_->getMotor().setVelocity(velocity);
    }

    bool CollisionResolver::hasBackObstacle() const
    {
        return obstacleDetector_.check(M_PI - (BACK_OBSTACLE_FOV / 2),
                                       M_PI + (BACK_OBSTACLE_FOV / 2),
                                       backStopDistance_);
    }


}
