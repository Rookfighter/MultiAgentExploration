#include "control/SimulationLoader.hpp"

namespace mae
{

	SimulationLoader::SimulationLoader()
	:client_(NULL), simulation_(NULL), stock_(NULL)
	{
	}

	SimulationLoader::~SimulationLoader()
	{
		cleanup();
	}

	void SimulationLoader::load(const std::string& p_file)
	{
		cleanup();
	}
	
	void SimulationLoader::cleanup()
	{
		for(ExplorationBot *robot : robots_)
			delete robot;
		robots_.clear();
		
		if(stock_ != NULL) {
			delete stock_;
			stock_ = NULL;
		}
		if(simulation_ != NULL) {
			delete simulation_;
			simulation_ = NULL;
		}
		if(client_ != NULL) {
			delete client_;
			client_ = NULL;
		}
	}
		
	PlayerClient* SimulationLoader::getClient()
	{
		return client_;
	}
	
	Simulation* SimulationLoader::getSimulation()
	{
		return simulation_;
	}
	
	MarkerStock* SimulationLoader::getStock()
	{
		return stock_;
	}
	
	const std::vector<ExplorationBot*>& SimulationLoader::getRobots()
	{
		return robots_;
	}
	
	ExplorationBot* SimulationLoader::getRobot(const std::string &p_name)
	{
		ExplorationBot *result = NULL;
		for(ExplorationBot *robot : robots_) {
			if(robot->getName() == p_name) {
				result = robot;
				break;
			}
		}
		
		return result;
	}
}
