#ifndef TUT_MEYBOT_HPP
#define TUT_MEYBOT_HPP

#include "control/ExplorationBot.hpp"

namespace mae
{
	
	enum MeybotSonar {FRONT=0, LEFT, RIGHT};
	
	class MeybotConfiguration : public ExplorationBotConfiguration
	{
	public:
		int rangerIndex;
	};
	
	class Meybot : public ExplorationBot
	{
	protected:
		PlayerCc::RangerProxy ranger_;
	public:
		Meybot(const MeybotConfiguration &p_config);
		~Meybot();
		
		double getSonarDistance(const int p_sensor) const;
		int getSonarCount() const;
	};

}

#endif
