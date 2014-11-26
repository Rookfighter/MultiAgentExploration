#include "io/ExperimentLoader.hpp"

#include <yaml-cpp/yaml.h>
#include <easylogging++.h>
#include "io/YamlNode.hpp"
#include "algorithm/RandomWalk.hpp"
#include "algorithm-rt/NodeCounting.hpp"
#include "algorithm-rt/LRTAStar.hpp"
#include "algorithm-rt/Wagner.hpp"
#include "algorithm-compass/CompassAlgorithm.hpp"
#include "utils/Convert.hpp"

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

	void ExperimentLoader::load(const std::string &p_file)
	{
        
		YAML::Node root;
        
		LOG(INFO) << "Loading config: " << p_file;
		root = YAML::LoadFile(p_file);
		LOG(INFO) << "-- file found";
		
		load(root);
	}
    
    void ExperimentLoader::load(YAML::Node &p_root)
    {
        YAML::Node algorithmNode, experimentNode, tmpNode;
		
        simulationLoader_.load(p_root);
        statisticLoader_.load(p_root);

		algorithmNode = p_root[YamlNode::algorithm];
        if(!algorithmNode.IsDefined())
            throw std::logic_error("algorithm node not found");
		LOG(INFO) <<  "-- algorithm config found";
		
		algorithmConfig_.type = toLowerCase(algorithmNode[YamlNode::type].as<std::string>());
        algorithmConfig_.obstacleStopDistance = algorithmNode[YamlNode::obstacleStopDistance].as<double>();
		algorithmConfig_.obstacleAvoidDistance = algorithmNode[YamlNode::obstacleAvoidDistance].as<double>();
		algorithmConfig_.obstacleMarkerDistance = algorithmNode[YamlNode::obstacleMarkerDistance].as<double>();
        algorithmConfig_.markerDeployDistance = algorithmNode[YamlNode::markerDeployDistance].as<double>();
        algorithmConfig_.collisionResolveDistance = algorithmNode[YamlNode::collisionResolveDistance].as<double>();
		LOG(INFO) <<  "-- algorithm type is " << algorithmConfig_.type;
		
		experimentNode = p_root[YamlNode::experiment];
		if(!experimentNode.IsDefined())
		    throw std::logic_error("experiment node not found");
		LOG(INFO) << "-- experiment config found";

		tmpNode = experimentNode[YamlNode::terminationMinutes];
		if(tmpNode.IsDefined()) {
		    experimentConfig_.terminationMinutes = tmpNode.as<int>();
		    LOG(INFO) << "-- terminationMinutes: " << experimentConfig_.terminationMinutes;
		}

		tmpNode = experimentNode[YamlNode::terminationCoverage];
		if(tmpNode.IsDefined()) {
		    experimentConfig_.terminationCoverage= tmpNode.as<double>();
		    LOG(INFO) << "-- terminationCoverage: " << experimentConfig_.terminationCoverage;
		}
    }

    Experiment* ExperimentLoader::create()
    {
        Simulation *simulation = simulationLoader_.create();
        Statistic *statistic = statisticLoader_.create(simulation);

        algorithmConfig_.stock = simulation->getStock();
        experimentConfig_.simulation = simulation;
        experimentConfig_.statistic = statistic;

        experimentConfig_.algorithms.resize(simulation->getRobots().size());
        for(unsigned int i = 0; i < experimentConfig_.algorithms.size(); ++i) {
            algorithmConfig_.robot = simulation->getRobots()[i];

            if(algorithmConfig_.type == NODECOUNTING_TYPE) {
                experimentConfig_.algorithms[i] = new NodeCounting(algorithmConfig_);
            } else if(algorithmConfig_.type == LRTASTAR_TYPE) {
                experimentConfig_.algorithms[i] = new LRTAStar(algorithmConfig_);
            } else if(algorithmConfig_.type == WAGNER_TYPE){
                experimentConfig_.algorithms[i] = new Wagner(algorithmConfig_);
            } else if (algorithmConfig_.type == RANDOMWALK_TYPE) {
                experimentConfig_.algorithms[i] = new RandomWalk(algorithmConfig_);
            } else if(algorithmConfig_.type == COMPASS_TYPE) {
                experimentConfig_.algorithms[i] = new CompassAlgorithm(algorithmConfig_);
            } else {
                throw std::logic_error("invalid Algorithm type");
            }
        }

        Experiment *result = new Experiment(experimentConfig_);

        return result;
    }

}
