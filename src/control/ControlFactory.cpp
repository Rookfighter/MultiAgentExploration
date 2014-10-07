#include "control/ControlFactory.hpp"

namespace mae
{

	ControlFactory::ControlFactory(const SimulationConfiguration &p_config)
	{
		simulation_ = new Simulation(p_config);
	}

	ControlFactory::~ControlFactory()
	{
		delete simulation_;
	}

	Simulation* ControlFactory::getSimulation()
	{
		return simulation_;
	}

	std::shared_ptr<Meybot> ControlFactory::createMeybot(const std::string &p_name,
	        const int p_motorIndex,
	        const int p_rangerIndex)
	{
		MeybotConfiguration config;
		config.name = p_name;
		config.motorIndex = p_motorIndex;
		config.rangerIndex = p_rangerIndex;
		config.simulation = simulation_;
		
		return std::shared_ptr<Meybot>(new Meybot(config));
	}
}
