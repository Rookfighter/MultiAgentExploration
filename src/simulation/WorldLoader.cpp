#include <yaml-cpp/yaml.h>
#include <easylogging++.h>
#include "simulation/WorldLoader.hpp"

#define CLIENT_NODE "client"
#define SIMULATION_NODE "simulation"
#define STOCK_NODE "stock"
#define ROBOTS_NODE "robots"

#define HOST_NODE "host"
#define PORT_NDOE "port"

#define MARKER_NAME_NODE "marker_name"
#define MARKER_COUNT_NODE "marker_count"
#define MARKER_RANGE_NODE "marker_range"

#define SIMULATION_INDEX_NODE "simulation_index"

#define NAME_NODE "name"
#define MOTOR_INDEX_NODE "motor_index"
#define RANGER_INDEX_NODE "ranger_index"
#define MAX_VELOCITY_NODE "max_velocity"
#define MIN_VELOCITY_NODE "min_velocity"

namespace mae
{

	WorldLoader::WorldLoader()
	{
	}

	WorldLoader::~WorldLoader()
	{
	}

	World* WorldLoader::load(const std::string &p_file)
	{
		LOG(DEBUG) << "Loading config: " << p_file;
		
		YAML::Node config, clientNode, simulationNode, stockNode, robotsNode;
		std::string host;
		int port;
		SimulationConfig simulationConfig;
		StockConfig stockConfig;
		std::vector<RobotConfig> robotConfigs;
		
		config = YAML::LoadFile(p_file);
		LOG(DEBUG) << "-- file found";
		
		// get configuration of client
		clientNode = config[CLIENT_NODE];
		host = clientNode[HOST_NODE].as<std::string>();
		port = clientNode[PORT_NDOE].as<int>();
		LOG(DEBUG) << "-- client info found";
		
		// get configuration of simulation
		simulationNode = config[SIMULATION_NODE];
		simulationConfig.simulationIndex = simulationNode[SIMULATION_INDEX_NODE].as<int>();
		LOG(DEBUG) << "-- simulation info found";
		
		// get configuration of marker stock
		stockNode = config[STOCK_NODE];
		stockConfig.markerName = stockNode[MARKER_NAME_NODE].as<std::string>();
		stockConfig.markerCount = stockNode[MARKER_COUNT_NODE].as<int>();
		stockConfig.markerRange = stockNode[MARKER_RANGE_NODE].as<double>();
		LOG(DEBUG) << "-- stock info found";
		
		// get configuration for all robots
		robotsNode = config[ROBOTS_NODE];
		LOG(DEBUG) << "-- robot info found";
		if(robotsNode.IsSequence()) {
			LOG(DEBUG) << "-- is sequence: " << robotsNode.size();
			
			robotConfigs.resize(robotsNode.size());
			for(int i = 0; i < robotsNode.size(); ++i) {
				robotConfigs[i].name = robotsNode[i][NAME_NODE].as<std::string>();
				robotConfigs[i].motorIndex = robotsNode[i][MOTOR_INDEX_NODE].as<int>();
				robotConfigs[i].rangerIndex = robotsNode[i][RANGER_INDEX_NODE].as<int>();

				assert(robotsNode[i][MIN_VELOCITY_NODE].IsSequence() &&
				       robotsNode[i][MIN_VELOCITY_NODE].size() == 2);
				robotConfigs[i].minVelocity.set(robotsNode[i][MIN_VELOCITY_NODE][0].as<double>(),
				                                 robotsNode[i][MIN_VELOCITY_NODE][1].as<double>());
												 
				assert(robotsNode[i][MAX_VELOCITY_NODE].IsSequence() &&
				       robotsNode[i][MAX_VELOCITY_NODE].size() == 2);
				robotConfigs[i].maxVelocity.set(robotsNode[i][MAX_VELOCITY_NODE][0].as<double>(),
				                                 robotsNode[i][MAX_VELOCITY_NODE][1].as<double>());
			}
		}
		
		World *result = new World;
		
		result->client_ = new PlayerClient(host, port);
		
		simulationConfig.client = result->client_;
		result->simulation_ = new Simulation(simulationConfig);
		
		stockConfig.simulation = result->simulation_;
		result->stock_ = new MarkerStock(stockConfig);
		
		result->robots_.resize(robotConfigs.size());
		for(int i = 0; i < result->robots_.size(); ++i) {
			robotConfigs[i].client = result->client_;
			robotConfigs[i].simulation = result->simulation_;
			robotConfigs[i].stock = result->stock_;
			result->robots_[i] = new ExplorationBot(robotConfigs[i]);
		}
		
		return result;
	}
}
