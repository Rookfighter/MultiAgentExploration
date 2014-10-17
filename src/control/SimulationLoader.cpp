#include <yaml-cpp/yaml.h>
#include "control/SimulationLoader.hpp"

#define CLIENT_NODE "client"
#define SIMULATION_NODE "simulation"
#define STOCK_NODE "stock"
#define ROBOTS_NODE "robots"

#define HOST_NODE "host"
#define PORT_NDOE "port"

#define MARKER_NAME_NODE "marker_name"
#define MARKER_COUNT_NODE "marker_count"

#define SIMULATION_INDEX_NODE "simulation_index"

#define NAME_NODE "name"
#define MOTOR_INDEX_NODE "motor_index"
#define RANGER_INDEX_NODE "ranger_index"
#define MAX_VELOCITY_NODE "max_velocity"
#define MIN_VELOCITY_NODE "min_velocity"

namespace mae
{

	SimulationLoader::SimulationLoader()
		:client_(NULL), simulation_(NULL), stock_(NULL), robots_()
	{
	}

	SimulationLoader::~SimulationLoader()
	{
		cleanup();
	}

	void SimulationLoader::load(const std::string& p_file)
	{
		cleanup();

		YAML::Node config = YAML::LoadFile(p_file);
		
		// get configuration of client
		YAML::Node clientNode = config[CLIENT_NODE];
		host_ = clientNode[HOST_NODE].as<std::string>();
		port_ = clientNode[PORT_NDOE].as<int>();
		
		// get configuration of simulation
		YAML::Node simulationNode = config[SIMULATION_NODE];
		simulationConfig_.simulationIndex = simulationNode[SIMULATION_INDEX_NODE].as<int>();
		
		// get configuration of marker stock
		YAML::Node stockNode = config[STOCK_NODE];
		stockConfig_.markerName = stockNode[MARKER_NAME_NODE].as<std::string>();
		stockConfig_.markerCount = stockNode[MARKER_COUNT_NODE].as<int>();
		
		// get configuration for all robots
		YAML::Node robotsNode = config[ROBOTS_NODE];
		if(robotsNode.IsSequence()) {
			robotConfigs_.resize(robotsNode.size());
			for(int i = 0; i < robotsNode.size(); ++i) {
				robotConfigs_[i].name = robotsNode[i][NAME_NODE].as<std::string>();
				robotConfigs_[i].motorIndex = robotsNode[i][MOTOR_INDEX_NODE].as<int>();
				robotConfigs_[i].rangerIndex = robotsNode[i][RANGER_INDEX_NODE].as<int>();

				assert(robotsNode[i][MIN_VELOCITY_NODE].IsSequence() &&
				       robotsNode[i][MIN_VELOCITY_NODE].size() == 2);
				robotConfigs_[i].minVelocity.set(robotsNode[i][MIN_VELOCITY_NODE][0].as<double>(),
				                                 robotsNode[i][MIN_VELOCITY_NODE][1].as<double>());
												 
				assert(robotsNode[i][MAX_VELOCITY_NODE].IsSequence() &&
				       robotsNode[i][MAX_VELOCITY_NODE].size() == 2);
				robotConfigs_[i].maxVelocity.set(robotsNode[i][MAX_VELOCITY_NODE][0].as<double>(),
				                                 robotsNode[i][MAX_VELOCITY_NODE][1].as<double>());
			}
		}
		
		init();
	}
	
	void SimulationLoader::init()
	{
		client_ = new PlayerClient(host_, port_);
		
		simulationConfig_.client = client_;
		simulation_ = new Simulation(simulationConfig_);
		
		stockConfig_.simulation = simulation_;
		stock_ = new MarkerStock(stockConfig_);
		
		robots_.resize(robotConfigs_.size());
		for(int i = 0; i < robots_.size(); ++i) {
			robotConfigs_[i].client = client_;
			robotConfigs_[i].simulation = simulation_;
			robots_[i] = new ExplorationBot(robotConfigs_[i]);
		}
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
