#include <easylogging++.h>
#include "simulation/WorldLoader.hpp"

#define WORLD_NODE "world"
#define WORLD_FILE_NODE "file"

#define STOCK_NODE "stock"
#define ROBOTS_NODE "robots"

#define HOST_NODE "host"
#define PORT_NDOE "port"

#define REFILL_COUNT_NODE "refill_count"

#define NAME_NODE "name"
#define MOTOR_INDEX_NODE "motor_index"
#define RANGER_INDEX_NODE "ranger_index"
#define MAX_VELOCITY_NODE "max_velocity"
#define MIN_VELOCITY_NODE "min_velocity"
#define MARKER_SENSOR_MAX_RANGE_NODE "marker_sensor_max_range"

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
		YAML::Node root;
		LOG(INFO) << "Loading config: " << p_file;
		root = YAML::LoadFile(p_file);
		LOG(INFO) << "-- file found";
		return load(root);
	}
	
	World* WorldLoader::load(YAML::Node& p_root)
	{
		YAML::Node worldNode, stockNode, robotsNode;
		std::string worldFile;
		StockConfig stockConfig;
		std::vector<RobotConfig> robotConfigs;
		
		// get configuration of client
		worldNode = p_root[WORLD_NODE];
		worldFile = clientNode[WORLD_FILE_NODE].as<std::string>();
		LOG(INFO) << "-- world config found";
		
		// get configuration of marker stock
		stockNode = p_root[STOCK_NODE];
		stockConfig.refillCount = stockNode[REFILL_COUNT_NODE].as<int>();
		LOG(INFO) << "-- stock config found";
		
		// get configuration for all robots
		robotsNode = p_root[ROBOTS_NODE];
		LOG(INFO) << "-- robot config found";
		if(robotsNode.IsSequence()) {
			LOG(INFO) << "-- found " << robotsNode.size() << " robot configs";
			
			robotConfigs.resize(robotsNode.size());
			for(int i = 0; i < robotsNode.size(); ++i) {
				robotConfigs[i].name = robotsNode[i][NAME_NODE].as<std::string>();
				robotConfigs[i].rangerIndex = robotsNode[i][RANGER_INDEX_NODE].as<int>();
				robotConfigs[i].markerSensorMaxRange = robotsNode[i][MARKER_SENSOR_MAX_RANGE_NODE].as<double>();
				
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
		
		result->world_ = Stg::WorldGui();
		result->world_.Load(worldFile);
		
		stockConfig.world = result->world_;
		result->stock_ = new MarkerStock(stockConfig);
		
		result->robots_.resize(robotConfigs.size());
		for(int i = 0; i < result->robots_.size(); ++i) {
			robotConfigs[i].world = result->world_;
			robotConfigs[i].stock = result->stock_;
			result->robots_[i] = new ExplorationBot(robotConfigs[i]);
		}
		
		return result;
	}
}
