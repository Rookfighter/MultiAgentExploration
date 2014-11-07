#ifndef MAE_UPDATING_VALUE_HPP
#define MAE_UPDATING_VALUE_HPP

#include "algorithm-rt/AntState.hpp"

namespace mae
{

	class UpdatingValue : public AntState
	{
	public:
		UpdatingValue(const AntStateProperties &p_properties);
		~UpdatingValue();
		
		AntState* update();

	};

}

#endif
