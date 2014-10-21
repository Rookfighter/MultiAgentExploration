#ifndef MAE_INITIAL_ANT_STATE_HPP
#define MAE_INITIAL_ANT_STATE_HPP

#include "algorithm/AntState.hpp"

namespace mae
{

	class InitialAntState : public AntState
	{
	public:
		InitialAntState(const AntStateProperties &p_properties);
		~InitialAntState();

		AntState* update();
	};

}

#endif
