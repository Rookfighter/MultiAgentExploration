#include <yaml-cpp/yaml.h>
#include <easylogging++.h>
#include "loading/ExperimentLoader.hpp"
#include "algorithm/RandomWalk.hpp"
#include "algorithm-rt/NodeCounting.hpp"
#include "algorithm-rt/LRTAStar.hpp"
#include "algorithm-rt/Wagner.hpp"
#include "algorithm-compass/CompassAlgorithm.hpp"
#include "loading/SimulationLoader.hpp"
#include "utils/Convert.hpp"
#include "loading/YamlNode.hpp"

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
        YAML::Node algorithmNode, experimentNode;
		AlgorithmConfig algorithmConfig;
		ExperimentConfig experimentConfig;
		Simulation *simulation;
		
		algorithmNode = p_root[YamlNode::algorithm];
        if(!algorithmNode.IsDefined())
            throw std::logic_error("algorithm node not found");
		LOG(INFO) <<  "-- algorithm config found";
		
		algorithmConfig.type = toLowerCase(algorithmNode[YamlNode::type].as<std::string>());
        algorithmConfig.obstacleStopDistance = algorithmNode[YamlNode::obstacleStopDistance].as<double>();
		algorithmConfig.obstacleAvoidDistance = algorithmNode[YamlNode::obstacleAvoidDistance].as<double>();
		algorithmConfig.obstacleMarkerDistance = algorithmNode[YamlNode::obstacleMarkerDistance].as<double>();
        algorithmConfig.markerDeployDistance = algorithmNode[YamlNode::markerDeployDistance].as<double>();
        algorithmConfig.collisionResolveDistance = algorithmNode[YamlNode::collisionResolveDistance].as<double>();
		LOG(INFO) <<  "-- algorithm type is " << algorithmConfig.type;
		
		experimentNode = p_root[YamlNode::experiment];
		if(!experimentNode.IsDefined())
		    throw std::logic_error("experiment node not found");
		LOG(INFO) << "-- experiment config found";

		experimentConfig.terminationMinutes = experimentNode[YamlNode::terminationMinutes].as<int>();

		simulation = SimulationLoader::load(p_root);
		algorithmConfig.stock = simulation->getStock();
		
		experimentConfig.simulation = simulation;
		
		experimentConfig.algorithms.resize(simulation->getRobots().size());
		for(unsigned int i = 0; i < experimentConfig.algorithms.size(); ++i) {
			algorithmConfig.robot = simulation->getRobots()[i];
			
			if(algorithmConfig.type == NODECOUNTING_TYPE) {
			    experimentConfig.algorithms[i] = new NodeCounting(algorithmConfig);
			} else if(algorithmConfig.type == LRTASTAR_TYPE) {
			    experimentConfig.algorithms[i] = new LRTAStar(algorithmConfig);
			} else if(algorithmConfig.type == WAGNER_TYPE){
			    experimentConfig.algorithms[i] = new Wagner(algorithmConfig);
			} else if (algorithmConfig.type == RANDOMWALK_TYPE) {
			    experimentConfig.algorithms[i] = new RandomWalk(algorithmConfig);
			} else if(algorithmConfig.type == COMPASS_TYPE) {
			    experimentConfig.algorithms[i] = new CompassAlgorithm(algorithmConfig);
            } else {
				throw std::logic_error("invalid Algorithm type");
			}
		}
		
		Experiment *result = new Experiment(experimentConfig);

		return result;
    }
}
