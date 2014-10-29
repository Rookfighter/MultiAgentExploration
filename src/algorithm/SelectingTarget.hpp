#ifndef MAE_SELECTING_TARGET_HPP
#define MAE_SELECTING_TARGET_HPP

#include "algorithm/AntState.hpp"
#include "common/StopWatch.hpp"

namespace mae
{

	class SelectingTarget : public AntState
	{
	private:
		std::list<Marker*> markerInRange_;
		
		StopWatch watch_;
	
		void getMarkerInRange();
		bool checkBlankSpace();
		void getMarkerTarget();
	public:
		SelectingTarget(const AntStateProperties &p_properties);
		~SelectingTarget();
		
		AntState* update();
	};

}

#endif
