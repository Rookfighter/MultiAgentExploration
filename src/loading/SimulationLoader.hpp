#ifndef MAE_WORLD_LOADER_HPP
#define MAE_WORLD_LOADER_HPP

#include <yaml-cpp/yaml.h>
#include "simulation/Simulation.hpp"

namespace mae
{

	class SimulationLoader
	{
	private:
	    StockConfig stockConfig_;
        std::vector<RobotConfig> robotConfigs_;
        bool worldGui_;
        std::string worldFile_;
	public:
	    SimulationLoader();
        ~SimulationLoader();

		void load(const std::string &p_file);
		void load(YAML::Node &p_root);

		Simulation* create();
	};

}

#endif
