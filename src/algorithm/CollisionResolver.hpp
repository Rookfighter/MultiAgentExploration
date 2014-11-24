#ifndef MAE_COLLISION_RESOLVER_HPP
#define MAE_COLLISION_RESOLVER_HPP

#include "simulation/ExplorationBot.hpp"

namespace mae
{
    class CollisionResolver
    {
    private:
        ExplorationBot *robot_;
        Pose lastPose_;
        double backwardsDistance_;
        double distanceToMove_;
        
        void updateGeometry();
        void moveBackwards();
    public:
        CollisionResolver(ExplorationBot *p_robot,
                          const double p_backwardsDistance);
        ~CollisionResolver();

        void resolveCollision();
        bool finished();

        void update();

    };

}

#endif
