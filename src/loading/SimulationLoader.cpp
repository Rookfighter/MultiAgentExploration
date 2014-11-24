#include <easylogging++.h>
#include "loading/SimulationLoader.hpp"
#include "loading/YamlNode.hpp"

namespace mae
{

	SimulationLoader::SimulationLoader()
	{
	}

	SimulationLoader::~SimulationLoader()
	{
	}

	Simulation* SimulationLoader::load(const std::string &p_file)
	{
		YAML::Node root;
		LOG(INFO) << "Loading config: " << p_file;
		root = YAML::LoadFile(p_file);
		LOG(INFO) << "-- file found";
		return load(root);
	}
	
	Simulation* SimulationLoader::load(YAML::Node& p_root)
	{
		YAML::Node worldNode, stockNode, robotsNode;
		std::string worldFile;
        bool worldGui;
		StockConfig stockConfig;
		std::vector<RobotConfig> robotConfigs;
		
		// get configuration of client
		worldNode = p_root[YamlNode::world];
        if(!worldNode.IsDefined())
            throw std::logic_error("world node not found");
		worldFile = worldNode[YamlNode::worldFile].as<std::string>();
        worldGui = worldNode[YamlNode::worldGui].as<bool>();
		LOG(INFO) << "-- world config found";
		
		// get configuration of marker stock
		stockNode = p_root[YamlNode::stock];
        if(!stockNode.IsDefined())
            throw std::logic_error("stock node not found");
		stockConfig.refillCount = stockNode[YamlNode::refillCount].as<int>();
		LOG(INFO) << "-- stock config found";
		
		// get configuration for all robots
		robotsNode = p_root[YamlNode::robots];
        if(!robotsNode.IsDefined())
            throw std::logic_error("robots node not found");
		LOG(INFO) << "-- robot config found";
		if(robotsNode.IsSequence()) {
			LOG(INFO) << "-- found " << robotsNode.size() << " robot configs";
			
			robotConfigs.resize(robotsNode.size());
			for(unsigned int i = 0; i < robotsNode.size(); ++i) {
				robotConfigs[i].name = robotsNode[i][YamlNode::name].as<std::string>();
				robotConfigs[i].rangerIndex = robotsNode[i][YamlNode::rangerIndex].as<int>();
				robotConfigs[i].markerSensorMaxRange = robotsNode[i][YamlNode::markerSensorMaxRange].as<double>();
				
				assert(robotsNode[i][YamlNode::minVelocity].IsSequence() &&
				       robotsNode[i][YamlNode::minVelocity].size() == 2);
				robotConfigs[i].minVelocity.set(robotsNode[i][YamlNode::minVelocity][0].as<double>(),
				                                 robotsNode[i][YamlNode::minVelocity][1].as<double>());
												 
				assert(robotsNode[i][YamlNode::maxVelocity].IsSequence() &&
				       robotsNode[i][YamlNode::maxVelocity].size() == 2);
				robotConfigs[i].maxVelocity.set(robotsNode[i][YamlNode::maxVelocity][0].as<double>(),
				                                 robotsNode[i][YamlNode::maxVelocity][1].as<double>());
			}
		}
			
		Simulation *result = new Simulation;
		
        if(worldGui)
            result->world_ = new Stg::WorldGui(600, 600, "Multi Agent Exploration");
        else
            result->world_ = new Stg::World("Multi Agent Exploration");
		result->world_->Load(worldFile);
		
		stockConfig.world = result->world_;
		result->stock_ = new MarkerStock(stockConfig);
		
		result->robots_.resize(robotConfigs.size());
		for(unsigned int i = 0; i < result->robots_.size(); ++i) {
			robotConfigs[i].world = result->world_;
			robotConfigs[i].stock = result->stock_;
			result->robots_[i] = new ExplorationBot(robotConfigs[i]);
		}
		
		return result;
	}
}
