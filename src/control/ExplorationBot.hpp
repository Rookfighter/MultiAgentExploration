#ifndef MAE_ANT_BOT_HPP
#define MAE_ANT_BOT_HPP

#include <libplayerc++/playerc++.h>
#include <string>
#include "common/Odometry.hpp"
#include "control/Simulation.hpp"

namespace mae
{
	
	class ExplorationBotConfiguration
	{
	public:
		std::string name;
		Simulation *simulation;
		int motorIndex;
	};

	class ExplorationBot
	{
	protected:
		std::string name_;
		Simulation *simulation_;
		PlayerCc::Position2dProxy motor_;
		
	public:
		ExplorationBot(const ExplorationBotConfiguration &p_config);
		~ExplorationBot();
		
		void setPose(const Pose &p_pose);
		Pose getPose() const;
		Pose getAbsolutePose();
		
		void setVelocity(const Velocity &p_velocity);
		Velocity getVelocity() const;
		
		std::string getName() const;
		
		void placeMarker();
	};

}

#endif
