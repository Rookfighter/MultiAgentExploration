#ifndef MAE_AT_MARKER_HPP
#define MAE_AT_MARKER_HPP

#include "algorithm/MovementController.hpp"
#include "algorithm/ObstacleDetector.hpp"
#include "algorithm-compass/CompassState.hpp"

namespace mae
{

	class AtMarker : public CompassState
	{
	private:
        MovementController movementController_;
        ObstacleDetector obstacleDetector_;
        
        bool checkingRecommendedDirection_;
        
		void setRecommendedDirection();
        bool checkFrontObstacle();
	public:
		AtMarker(const CompassStateProperties &p_properties);
		~AtMarker();
		
		CompassState* update();
	};

}

#endif
