#include "control/Meybot.hpp"

namespace mae
{
	Meybot::Meybot(const MeybotConfiguration &p_config)
	:ExplorationBot(p_config), ranger_(simulation_->getClient(), p_config.rangerIndex)
	{
		ranger_.RequestGeom();
	}

	Meybot::~Meybot()
	{
		
	}
	
	double Meybot::getSonarDistance(const int p_sensor) const
	{
		return ranger_[p_sensor];
	}
	
	int Meybot::getSonarCount() const
	{
		return 3;
	}
}
