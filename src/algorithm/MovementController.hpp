#ifndef MAE_MOVEMENT_CONTROLLER_HPP
#define MAE_MOVEMENT_CONTROLLER_HPP

#include "algorithm/Wander.hpp"

namespace mae
{

    class MovementController
    {
    private:
        Wander wander_;
        ExplorationBot *robot_;
        Pose lastPose_;

        double angleToTurn_;
        double distanceToMove_;

        double angleEps_;

        double turnFactor_;

        void updateGeometry();
        void turn();
        void wander();
    public:
        MovementController(ExplorationBot *p_robot,
                           const double p_obstacleStopDistance,
                           const double p_obstacleAvoidDistance);
        ~MovementController();

        void setAngleEps(const double p_angleEps);
        void setTurnFactor(const double p_factor);

        void turnBy(const double p_angle);
        void wanderDistance(const double p_distance);

        bool reachedDistance() const;
        bool reachedDirection() const;
        bool finished() const;

        void update();
        void refreshPose();

    };

}

#endif
