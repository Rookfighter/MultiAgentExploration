#ifndef MAE_WORLD_LOADER_HPP
#define MAE_WORLD_LOADER_HPP

#include <yaml-cpp/yaml.h>
#include "simulation/World.hpp"

namespace mae
{

	class WorldLoader
	{
	private:
		WorldLoader();
		~WorldLoader();
	public:
		
		static World* load(const std::string &p_file);
		static World* load(YAML::Node& p_root);
	};

}

#endif
