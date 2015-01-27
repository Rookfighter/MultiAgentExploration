#ifndef MAE_MOVEMENT_CONTROLLER_HPP
#define MAE_MOVEMENT_CONTROLLER_HPP

#include "algorithm/Wander.hpp"
#include "algorithm/CollisionResolver.hpp"

namespace mae
{

    class MovementController
    {
    private:
        Wander wander_;
        CollisionResolver collisionResolver_;
        ExplorationBot *robot_;
        Pose lastPose_;

        double angleToTurn_;
        double distanceToMove_;
        double minDistanceToMove_;

        double angleEps_;

        double turnFactor_;
        
        bool hasCollision() const;
        void resolveCollision();
        void updateGeometry();
        void turn();
        void wander();
    public:
        MovementController(ExplorationBot *p_robot,
                           const double p_obstacleStopDistance,
                           const double p_obstacleAvoidDistance,
                           const double p_collisionResolveDistance);
        ~MovementController();

        void setAngleEps(const double p_angleEps);
        void setTurnFactor(const double p_factor);

        void turnBy(const double p_angle);
        void wanderDistance(const double p_distance);
        void wanderMinDistance(const double p_distance);
        void stopWandering();
        void stopTurning();

        bool reachedDistance() const;
        bool reachedMinDistance() const;
        bool reachedDirection() const;
        bool finished() const;
        bool isAvoidingObstacle() const;

        void update();
        void refreshPose();

    };

}

#endif
