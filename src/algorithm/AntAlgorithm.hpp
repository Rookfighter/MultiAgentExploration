#ifndef MAE_ANT_ALGORITHM_HPP
#define MAE_ANT_ALGORITHM_HPP

#include "algorithm/AntState.hpp"

namespace mae
{
	
	class AntAlgorithm
	{
	private:
		AntState *state_;
	protected:
		void init(AntState *p_initialState);
	public:
		AntAlgorithm();
		virtual ~AntAlgorithm();
		
		void step();
	};

}

#endif
