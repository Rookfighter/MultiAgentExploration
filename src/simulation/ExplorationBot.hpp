#ifndef MAE_EXPLORATION_BOT_HPP
#define MAE_EXPLORATION_BOT_HPP

#include "simulation/Motor.hpp"
#include "simulation/Ranger.hpp"
#include "simulation/MarkerSensor.hpp"
#include "simulation/TwoBitCompass.hpp"

namespace mae
{
	class ExplorationBot
	{
	private:
		Stg::Model *model_;
		std::string name_;
		Pose absolutePose_;
		
		Motor motor_;
		Ranger ranger_;
		MarkerSensor markerSensor_;
		TwoBitCompass compass_;
		
		MarkerStock *stock_;
	public:
		ExplorationBot(const RobotConfig &p_config);
		~ExplorationBot();

		void setPose(const Pose &p_pose);
		Pose getAbsolutePose();
		
		Motor& getMotor();
		Ranger& getRanger();
		MarkerSensor& getMarkerSensor();
		TwoBitCompass& getCompass();

		std::string getName() const;

		Marker* dropMarker();
		
		void update();
	};

}

#endif
