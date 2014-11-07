#ifndef MAE_WORLD_LOADER_HPP
#define MAE_WORLD_LOADER_HPP

#include <yaml-cpp/yaml.h>
#include "simulation/Simulation.hpp"

namespace mae
{

	class SimulationLoader
	{
	private:
		SimulationLoader();
		~SimulationLoader();
	public:
		
		static Simulation* load(const std::string &p_file);
		static Simulation* load(YAML::Node& p_root);
	};

}

#endif
