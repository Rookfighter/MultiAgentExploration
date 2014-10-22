#ifndef MAE_WORLD_HPP
#define MAE_WORLD_HPP

#include "simulation/ExplorationBot.hpp"

namespace mae
{
	class WorldLoader;
	
	class World
	{
		friend WorldLoader
	private:
		PlayerClient *client_;
		Simulation *simulation_;
		MarkerStock *stock_;
		std::vector<ExplorationBot*> robots_;
		
		World();
	public:
		~World();
		
		PlayerClient* getClient();
		Simulation* getSimulation();
		MarkerStock* getMarkerStock();
		
		const std::vector<ExplorationBot*>& getRobots();
		ExplorationBot* getRobot(const std::string& p_name);
	};

}

#endif
