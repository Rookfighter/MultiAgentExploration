#ifndef MAE_WANDER_HPP
#define MAE_WANDER_HPP

#include <functional>
#include "algorithm/ObstacleDetector.hpp"
#include "simulation/ExplorationBot.hpp"

namespace mae
{
	
	class Wander
	{
	private:
		ExplorationBot *robot_;
		ObstacleDetector obstacleDetector_;
		Velocity velocity_;

		double minFrontDistance_;
		double minLeftDistance_;
		double minRightDistance_;

		double frontStopDistance_;
        double avoidDistance_;
		
		bool stopRobot_;
		
		int avoidStep_;
		static const int STEP_REFRESH_INTERVAL = 10;
		
		void checkObstacle();
        bool hasStopObstacle() const;
		void stopAndAvoidObstacle();
        void avoidNearbyObstacle();
        bool hasNearbyObstacle() const;
		void cruise();
		
	public:
		Wander(ExplorationBot *p_robot,
		       const double p_frontStopDistance,
               const double p_avoidDistance);
		~Wander();
        
        bool isAvoidingObstacle() const;
        
		void update();
	};

}

#endif
