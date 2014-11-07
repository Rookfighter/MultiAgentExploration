#ifndef MAE_ANT_ALGORITHM_HPP
#define MAE_ANT_ALGORITHM_HPP

#include "algorithm/Algorithm.hpp"
#include "algorithm-rt/AntState.hpp"

namespace mae
{
	
	class AntAlgorithm : public Algorithm
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
