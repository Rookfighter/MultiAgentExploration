#include "simulation/World.hpp"
#include <easylogging++.h>

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
	
	void World::update()
	{
		watch_.start();
		client_->update();
		watch_.stop();
		LOG(INFO) << "ClientUpdate: " << watch_.strMsec();
		
		watch_.start();
		for(ExplorationBot *robot: robots_)
			robot->update();
		watch_.stop();
		LOG(INFO) << "RobotUpdate: " << watch_.strMsec();
	}
}
