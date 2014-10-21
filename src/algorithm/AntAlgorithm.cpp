#include <easylogging++.h>
#include "algorithm/AntAlgorithm.hpp"

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

	void AntAlgorithm::step()
	{
		assert(state_ != NULL);
		
		AntState *newState = state_->update();
		if(newState != NULL) {
			LOG(DEBUG) << "-- Changing AntState.";
			delete state_;
			state_ = newState;
		}
	}


}
