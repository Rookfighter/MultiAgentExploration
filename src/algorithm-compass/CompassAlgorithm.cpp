#include "algorithm-compass/CompassAlgorithm.hpp"
#include "algorithm-compass/InitialCompassState.hpp"

namespace mae
{

    CompassAlgorithm::CompassAlgorithm(const AlgorithmConfig &p_config)
        :Algorithm(p_config),
         state_(NULL)
    {
        CompassStateProperties properties;

        properties.currentMarker = NULL;
        properties.robot = p_config.robot;
        properties.obstacleStopDistance = p_config.obstacleStopDistance;
        properties.obstacleAvoidDistance = p_config.obstacleAvoidDistance;
        properties.markerDeployDistance = p_config.markerDeployDistance;
        properties.collisionResolveDistance = p_config.collisionResolveDistance;
        
        state_ = new InitialCompassState(properties);
    }

    CompassAlgorithm::~CompassAlgorithm()
    {
        if(state_ != NULL)
            delete state_;
    }

    void CompassAlgorithm::update()
    {
        assert(state_ != NULL);

        CompassState *newState = state_->update();

        if(newState != NULL) {
            delete state_;
            state_ = newState;
        }
    }
}
