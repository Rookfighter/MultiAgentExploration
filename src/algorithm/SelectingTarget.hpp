#ifndef MAE_SELECTING_TARGET_HPP
#define MAE_SELECTING_TARGET_HPP

#include "algorithm/AntState.hpp"

namespace mae
{

	class SelectingTarget : public AntState
	{
	private:
		std::vector<MarkerMeasurement> markerInRange_;
		double obstacleThreshold_;
		
		void getMarkerInRange();
		bool checkBlankSpace();
		bool checkObstalce(const double p_beginAngle, const double p_endAngle);
		void getMarkerTarget();
	public:
		SelectingTarget(const AntStateProperties &p_properties);
		~SelectingTarget();
		
		AntState* update();
	};

}

#endif
