#ifndef MAE_WORLD_HPP
#define MAE_WORLD_HPP

#include "simulation/ExplorationBot.hpp"
#include "utils/StopWatch.hpp"

namespace mae
{
	class World
	{
		friend class WorldLoader;
	private:
		PlayerClient *client_;
		Simulation *simulation_;
		MarkerStock *stock_;
		std::vector<ExplorationBot*> robots_;
		StopWatch watch_;
		
		World();
	public:
		~World();
		
		PlayerClient* getClient();
		Simulation* getSimulation();
		MarkerStock* getStock();
		
		const std::vector<ExplorationBot*>& getRobots();
		ExplorationBot* getRobot(const std::string& p_name);
		
		void update();
	};

}

#endif
