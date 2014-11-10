#ifndef MAE_SELECTING_TARGET_HPP
#define MAE_SELECTING_TARGET_HPP

#include "algorithm-rt/AntState.hpp"
#include "algorithm/ObstacleDetector.hpp"

namespace mae
{

	class SelectingTarget : public AntState
	{
	private:
		std::vector<MarkerMeasurement> markerInRange_;
		ObstacleDetector obstacleDetector_;
		double obstacleMarkerDistance_;

		void getMarkerInRange();
		bool checkBlankSpace();
		bool findNextMarker();
		std::vector<MarkerMeasurement> getNonObstructedMarker();
		double getMinNonObstructedMarkerValue();
		bool markerIsObstructed(const MarkerMeasurement &p_markerMeasurement);
	public:
		SelectingTarget(const AntStateProperties &p_properties);
		~SelectingTarget();

		AntState* update();
	};

}

#endif
