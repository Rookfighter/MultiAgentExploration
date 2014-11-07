#include <easylogging++.h>
#include "simulation/World.hpp"

namespace mae
{

	World::World()
	: world_(NULL), stock_(NULL), robots_()
	{
	}

	World::~World()
	{
		for(ExplorationBot* robot: robots_)
			delete robot;
		robots_.clear();
		
		if(stock_ != NULL)
			delete stock_;
	}
		
	Stg::World* World::getWorld();
	{
		return world_;
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
	
	void World::update()
	{
		for(ExplorationBot *robot: robots_)
			robot->update();
		client_->update();
	}
}
