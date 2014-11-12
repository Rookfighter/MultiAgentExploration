#include "algorithm-compass/CompassAlgorithm.hpp"

namespace mae
{

	CompassAlgorithm::CompassAlgorithm(const AlgorithmConfig &p_config)
	:state_(NULL)
	{
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
