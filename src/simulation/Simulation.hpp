#ifndef MAE_WORLD_HPP
#define MAE_WORLD_HPP

#include "simulation/ExplorationBot.hpp"

namespace mae
{
	class Simulation
	{
		friend class SimulationLoader;
	private:
		Stg::World *world_;
		MarkerStock *stock_;
		std::vector<ExplorationBot*> robots_;
		
		Simulation();
	public:
		~Simulation();
		
		Stg::World* getWorld();
		MarkerStock* getStock();
		
		const std::vector<ExplorationBot*>& getRobots();
		ExplorationBot* getRobot(const std::string& p_name);
		
		void update();
	};

}

#endif
