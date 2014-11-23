#include <yaml-cpp/yaml.h>
#include <easylogging++.h>
#include "algorithm/ExperimentLoader.hpp"
#include "algorithm/RandomWalk.hpp"
#include "algorithm-rt/NodeCounting.hpp"
#include "algorithm-rt/LRTAStar.hpp"
#include "algorithm-rt/Wagner.hpp"
#include "algorithm-compass/CompassAlgorithm.hpp"
#include "simulation/SimulationLoader.hpp"
#include "utils/Convert.hpp"
#include "utils/YamlNode.hpp"

#define NODECOUNTING_TYPE "nodecounting"
#define LRTASTAR_TYPE "lrtastar"
#define WAGNER_TYPE "wagner"
#define RANDOMWALK_TYPE "randomwalk"
#define COMPASS_TYPE "compass"

namespace mae
{

	ExperimentLoader::ExperimentLoader()
	{
	}

	ExperimentLoader::~ExperimentLoader()
	{
	}

	Experiment* ExperimentLoader::load(const std::string &p_file)
	{
		YAML::Node root;
		
		LOG(INFO) << "Loading config: " << p_file;
		root = YAML::LoadFile(p_file);
		LOG(INFO) << "-- file found";
		
		return load(root);
	}
    
    Experiment* ExperimentLoader::load(YAML::Node &p_root)
    {
        YAML::Node algorithm;
		AlgorithmConfig algorithmConfig;
		Simulation *simulation;
		
		algorithm = p_root[YamlNode::algorithm];
		LOG(INFO) <<  "-- algorithm config found";
		
		algorithmConfig.type = toLowerCase(algorithm[YamlNode::type].as<std::string>());
        algorithmConfig.obstacleStopDistance = algorithm[YamlNode::obstacleStopDistance].as<double>();
		algorithmConfig.obstacleAvoidDistance = algorithm[YamlNode::obstacleAvoidDistance].as<double>();
		algorithmConfig.obstacleMarkerDistance = algorithm[YamlNode::obstacleMarkerDistance].as<double>();
        algorithmConfig.markerDeployDistance = algorithm[YamlNode::markerDeployDistance].as<double>();
		LOG(INFO) <<  "-- algorithm type is " << algorithmConfig.type;
		
		simulation = SimulationLoader::load(p_root);
		algorithmConfig.stock = simulation->getStock();
		
		Experiment *result = new Experiment();
		result->simulation_ = simulation;
		
		result->algorithms_.resize(simulation->getRobots().size());
		for(unsigned int i = 0; i < result->algorithms_.size(); ++i) {
			algorithmConfig.robot = simulation->getRobots()[i];
			
			if(algorithmConfig.type == NODECOUNTING_TYPE) {
				result->algorithms_[i] = new NodeCounting(algorithmConfig);
			} else if(algorithmConfig.type == LRTASTAR_TYPE) {
				result->algorithms_[i] = new LRTAStar(algorithmConfig);
			} else if(algorithmConfig.type == WAGNER_TYPE){
				result->algorithms_[i] = new Wagner(algorithmConfig);
			} else if (algorithmConfig.type == RANDOMWALK_TYPE) {
				result->algorithms_[i] = new RandomWalk(algorithmConfig);
			} else if(algorithmConfig.type == COMPASS_TYPE) {
                result->algorithms_[i] = new CompassAlgorithm(algorithmConfig);
            } else {
				throw std::logic_error("invalid Algorithm type");
			}
		}
		
		return result;
    }
}
