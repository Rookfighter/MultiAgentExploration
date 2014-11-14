#include <easylogging++.h>
#include "simulation/Simulation.hpp"

namespace mae
{

	Simulation::Simulation()
	: world_(NULL), stock_(NULL), robots_()
	{
	}

	Simulation::~Simulation()
	{
		for(ExplorationBot* robot: robots_)
			delete robot;
		robots_.clear();
		
		if(stock_ != NULL)
			delete stock_;
            
        if(world_ != NULL)    
            delete world_;
	}
		
	Stg::World* Simulation::getWorld()
	{
		return world_;
	}
	
	MarkerStock* Simulation::getStock()
	{
		return stock_;
	}
	
	const std::vector<ExplorationBot*>& Simulation::getRobots()
	{
		return robots_;
	}
	
	ExplorationBot* Simulation::getRobot(const std::string& p_name)
	{
		for(ExplorationBot* robot : robots_) {
			if(robot->getName() == p_name)
				return robot;
		}
		
		return NULL;
	}
}
