#include <easylogging++.h>
#include "algorithm-rt/AntAlgorithm.hpp"

namespace mae
{
	AntAlgorithm::AntAlgorithm()
	:state_(NULL)
	{
	}

	AntAlgorithm::~AntAlgorithm()
	{
		if(state_ != NULL)
		delete state_;
	}
	
	void AntAlgorithm::init(AntState *p_initialState)
	{
		state_ = p_initialState;
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
