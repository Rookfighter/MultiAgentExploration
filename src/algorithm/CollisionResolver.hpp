#ifndef MAE_COLLISION_RESOLVER_HPP
#define MAE_COLLISION_RESOLVER_HPP

#include "algorithm/ObstacleDetector.hpp"
#include "simulation/ExplorationBot.hpp"

namespace mae
{
    class CollisionResolver
    {
    private:
        ExplorationBot *robot_;
        ObstacleDetector obstacleDetector_;
        Pose lastPose_;
        
        double collisionResolveDistance_;
        double backStopDistance_;
        double distanceToMove_;
        
        void updateGeometry();
        void moveBackwards();
        bool hasBackObstacle() const;
    public:
        CollisionResolver(ExplorationBot *p_robot,
                          const double p_collisionResolveDistance,
                          const double p_backStopDistance);
        ~CollisionResolver();

        void resolveCollision();
        bool finished() const;

        void update();

    };

}

#endif
