#ifndef MAE_ANT_MARKER_HPP
#define MAE_ANT_MARKER_HPP

#include <string>
#include "common/Odometry.hpp"

namespace mae
{

	class AntMarker
	{
	public:
		std::string name;
		Pose pose;
		bool inUse;
		double range;
		int value;
		
		AntMarker();
		~AntMarker();

	};

}

#endif
