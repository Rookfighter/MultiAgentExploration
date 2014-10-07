#ifndef MAE_CONTROL_FACTORY_HPP
#define MAE_CONTROL_FACTORY_HPP

#include <libplayerc++/playerc++.h>
#include <memory>
#include "control/Simulation.hpp"
#include "control/Robots.hpp"

namespace mae
{

	class ControlFactory
	{
	private:
		Simulation *simulation_;
	public:
		ControlFactory(const SimulationConfiguration &p_config);
		~ControlFactory();
		
		Simulation* getSimulation();
		
		std::shared_ptr<Meybot> createMeybot(const std::string &p_name,
											 const int p_motorIndex,
											 const int p_rangerIndex);
	};

}

#endif
