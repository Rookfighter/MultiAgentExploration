#ifndef MAE_EXPLORATION_BOT_HPP
#define MAE_EXPLORATION_BOT_HPP

#include "simulation/Motor.hpp"
#include "simulation/Ranger.hpp"
#include "simulation/MarkerSensor.hpp"

namespace mae
{
	class ExplorationBot
	{
	private:
		std::string name_;
		Pose absolutePose_;
		
		Motor motor_;
		Ranger ranger_;
		MarkerSensor markerSensor_;
		
		Simulation *simulation_;
		MarkerStock *stock_;

	public:
		ExplorationBot(const RobotConfig &p_config);
		~ExplorationBot();

		void setPose(const Pose &p_pose);
		Pose getAbsolutePose();
		
		Motor& getMotor();
		Ranger& getRanger();
		MarkerSensor& getMarkerSensor();

		std::string getName() const;

		Marker* dropMarker();
		
		void update();
	};

}

#endif
