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

		double frontStopDistance_;
        double avoidDistance_;
		
		int avoidStep_;
		int stopStep_;
		static const int STOP_STEP_REFRESH_INTERVAL = 10;
		static const int AVOID_STEP_REFRESH_INTERVAL = 3;
		
		bool hasLeftObstacle() const;
		bool hasRightObstacle() const;
		bool hasfrontStopObstacle() const;
		double getLeftMinDistance() const;
		double getRightMinDistance() const;

		void checkObstacle();
		void stopAndAvoidObstacle();
        void avoidNearbyObstacle();
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
