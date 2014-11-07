#include <yaml-cpp/yaml.h>
#include <easylogging++.h>
#include "algorithm/ExperimentLoader.hpp"
#include "algorithm-rt/NodeCounting.hpp"
#include "simulation/SimulationLoader.hpp"
#include "utils/Convert.hpp"

#define ALGORITHM_NODE "algorithm"
#define ALGORITHM_TYPE_NODE "type"
#define OBSTACLE_AVOID_DISTANCE_NODE "obstacle_avoid_distance"
#define OBSTACLE_MARKER_DISTANCE_NODE "obstacle_marker_distance"

#define NODECOUNTING_TYPE "nodecounting"

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
		YAML::Node root, algorithm;
		AlgorithmConfig algorithmConfig;
		Simulation *simulation;
		
		LOG(INFO) << "Loading config: " << p_file;
		root = YAML::LoadFile(p_file);
		LOG(INFO) << "-- file found";
		
		algorithm = root[ALGORITHM_NODE];
		LOG(INFO) <<  "-- algorithm config found";
		
		algorithmConfig.type = toLowerCase(algorithm[ALGORITHM_TYPE_NODE].as<std::string>());
		algorithmConfig.obstacleAvoidDistance = algorithm[OBSTACLE_AVOID_DISTANCE_NODE].as<double>();
		algorithmConfig.obstacleMarkerDistance = algorithm[OBSTACLE_MARKER_DISTANCE_NODE].as<double>();
		
		simulation = SimulationLoader::load(root);
		algorithmConfig.stock = simulation->getStock();
		
		Experiment *result = new Experiment();
		result->simulation_ = simulation;
		
		result->algorithms_.resize(simulation->getRobots().size());
		for(int i = 0; i < result->algorithms_.size(); ++i) {
			algorithmConfig.robot = simulation->getRobots()[i];
			
			if(algorithmConfig.type == NODECOUNTING_TYPE) {
				result->algorithms_[i] = new NodeCounting(algorithmConfig);
			} else {
				throw std::logic_error("invalid Algorithm type");
			}
		}
		
		return result;
			
	}
}
