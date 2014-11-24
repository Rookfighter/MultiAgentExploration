#include "algorithm/CollisionResolver.hpp"

namespace mae
{

    CollisionResolver::CollisionResolver(ExplorationBot *p_robot,
                                         const double p_backwardsDistance)
        :robot_(p_robot),
         lastPose_(robot_->getMotor().getPose()),
         backwardsDistance_(p_backwardsDistance),
         distanceToMove_(0)
    {

    }

    CollisionResolver::~CollisionResolver()
    {

    }

    void CollisionResolver::resolveCollision()
    {
        distanceToMove_ = backwardsDistance_;
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
    }

    void CollisionResolver::moveBackwards()
    {
        Velocity velocity = robot_->getMotor().getMinVelocity();
        velocity.angular = 0;
        robot_->getMotor().setVelocity(velocity);
    }


}
