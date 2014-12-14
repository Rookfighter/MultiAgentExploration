#ifndef MAE_INITIAL_COMPASS_STATE_HPP
#define MAE_INITIAL_COMPASS_STATE_HPP

#include "algorithm-compass/CompassState.hpp"
#include "utils/Random.hpp"

namespace mae
{

    class InitialCompassState : public CompassState
    {
    public:
        InitialCompassState(const CompassStateProperties &p_properties);
        ~InitialCompassState();
        
        CompassState* update();
    };

}

#endif
