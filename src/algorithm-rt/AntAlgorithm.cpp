#include <easylogging++.h>
#include "algorithm-rt/AntAlgorithm.hpp"
#include "algorithm-rt/InitialAntState.hpp"

namespace mae
{
    AntAlgorithm::AntAlgorithm(const AlgorithmConfig &p_config,
                               std::function<void(Marker*,Marker*)> p_updateValue,
                               std::function<double(Marker*,Marker*)> p_calcValue)
        :Algorithm(p_config),
        state_(NULL)
    {
        AntStateProperties properties;
        properties.robot = p_config.robot;
        properties.updateValue = p_updateValue;
        properties.calcValue = p_calcValue;
        properties.obstacleStopDistance = p_config.obstacleStopDistance;
        properties.obstacleAvoidDistance = p_config.obstacleAvoidDistance;
        properties.obstacleMarkerDistance = p_config.obstacleMarkerDistance;
        properties.markerDeployDistance = p_config.markerDeployDistance;
        properties.collisionResolveDistance = p_config.collisionResolveDistance;

        state_ = new InitialAntState(properties);
    }

    AntAlgorithm::~AntAlgorithm()
    {
        if(state_ != NULL)
            delete state_;
    }

    void AntAlgorithm::update()
    {
        assert(state_ != NULL);

        AntState *newState = state_->update();

        if(newState != NULL) {
            delete state_;
            state_ = newState;
        }
    }


}
