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
		
		bool stopRobot_;
		
		int avoidStep_;
		static const int STEP_REFRESH_INTERVAL = 10;
		
		std::function<void(void)> avoidBeginCallback_;
		std::function<void(void)> avoidEndCallback_;
		
		void checkObstacle();
		void avoidObstacle();
		void cruise();
		
		void beginAvoiding();
		void endAvoiding();
	public:
		Wander(ExplorationBot *p_robot,
		       const double p_frontStopDistance = 1.0);
		~Wander();

		void step();
		
		void onAvoidBegin(std::function<void(void)> p_callback);
		void onAvoidEnd(std::function<void(void)> p_callback);
	};

}

#endif
