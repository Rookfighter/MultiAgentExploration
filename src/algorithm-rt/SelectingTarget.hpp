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
	    enum State {Init, CheckingSurrounding, SelectingBlank, TurningToBlank, CheckingFront, SelectingMarker, TurningToDirection};
	    struct DirectionInfo {
	        double direction;
	        std::vector<int> sensordIdx;
	        bool blocked;
	    };

		std::vector<MarkerMeasurement> markerInRange_;
		std::vector<DirectionInfo> directionsInfos_;
		State state_;
		int nextDirectionIdx_;
		ObstacleDetector obstacleDetector_;
		MovementController movementController_;
		double obstacleMarkerDistance_;
		double facingDirection_;

		void initDirectionInfos();
		AntState* executeState();
		void checkSourrounding();
		void getMarkerInRange();
		void checkBlankSpace();
		void selectBlankSpace();
		bool foundBlankSpace();
		void turnToBlank();
		void selectNextMarker();
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
