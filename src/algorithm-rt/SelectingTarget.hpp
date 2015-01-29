#ifndef MAE_SELECTING_TARGET_HPP
#define MAE_SELECTING_TARGET_HPP

#include "algorithm-rt/AntState.hpp"
#include "algorithm/ObstacleDetector.hpp"
#include "algorithm/MovementController.hpp"

namespace mae
{

	class SelectingTarget : public AntState
	{
	private:
	    struct DirectionInfo {
	        double direction;
	        std::vector<int> sensordIdx;
	        bool useable;
	    };

		std::vector<MarkerMeasurement> markerInRange_;
		std::vector<DirectionInfo> directionsInfos_;
		ObstacleDetector obstacleDetector_;
		MovementController movementController_;
		double obstacleMarkerDistance_;


		void initDirectionInfos();
		void checkSourrounding();
		void getMarkerInRange();
		bool checkBlankSpace();
		bool findNextMarker();
		std::vector<MarkerMeasurement> getPossibleTargets();
		double getMinNonObstructedMarkerValue();
        bool isPossibleTarget(const MarkerMeasurement &p_marker);
		bool markerIsObstructed(const MarkerMeasurement &p_markerMeasurement);
	public:
		SelectingTarget(const AntStateProperties &p_properties);
		~SelectingTarget();

		AntState* update();
	};

}

#endif
