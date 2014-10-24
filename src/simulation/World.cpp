#include "simulation/World.hpp"

namespace mae
{

	World::World()
	{
	}

	World::~World()
	{
		for(ExplorationBot* robot: robots_)
			delete robot;
		robots_.clear();
		
		delete stock_;
		delete simulation_;
		delete client_;
	}
		
	PlayerClient* World::getClient()
	{
		return client_;
	}
	
	Simulation* World::getSimulation()
	{
		return simulation_;
	}
	
	MarkerStock* World::getStock()
	{
		return stock_;
	}
	
	const std::vector<ExplorationBot*>& World::getRobots()
	{
		return robots_;
	}
	
	ExplorationBot* World::getRobot(const std::string& p_name)
	{
		for(ExplorationBot* robot : robots_) {
			if(robot->getName() == p_name)
				return robot;
		}
		
		return NULL;
	}
}
