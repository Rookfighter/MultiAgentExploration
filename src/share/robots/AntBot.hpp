#ifndef MAE_ANT_BOT_HPP
#define MAE_ANT_BOT_HPP

#include <libplayerc++/playerc++.h>
#include <string>
#include "common/Odometry.hpp"

namespace mae
{

	class AntBot
	{
	protected:
		std::string name_;
		PlayerCc::PlayerClient robot_;
		PlayerCc::Position2dProxy motor_;
		PlayerCc::SimulationProxy simulation_;
		
	public:
		AntBot(const std::string &p_name, const std::string &p_host, const int p_port);
		~AntBot();
		
		Pose getPose() const;
		Pose getAbsolutePose();
		
		void setVelocity(const Velocity &p_velocity);
		Velocity getVelocity() const;
		
		std::string getName() const;
		
		void update();
		
	};

}

#endif
