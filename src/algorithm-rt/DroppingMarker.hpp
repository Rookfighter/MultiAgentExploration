#ifndef MAE_DROPPING_MARKER_HPP
#define MAE_DROPPING_MARKER_HPP

#include "algorithm-rt/AntState.hpp"

namespace mae
{

	class DroppingMarker : public AntState
	{
	private:
	    bool hasTooCloseMarker() const;
	public:
		DroppingMarker(const AntStateProperties &p_properties);
		~DroppingMarker();

		AntState* update();
	};

}

#endif
