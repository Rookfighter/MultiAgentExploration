#ifndef MAE_ANT_MARKER_HPP
#define MAE_ANT_MARKER_HPP

#include <string>
#include "common/Odometry.hpp"

namespace mae
{

	class Marker
	{
	public:
		std::string name;
		Pose pose;
		bool inUse;
		int value;
		
		Marker();
		~Marker();

	};

}

#endif
