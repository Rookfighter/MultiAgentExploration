#include <yaml-cpp/yaml.h>
#include <easylogging++.h>
#include "algorithm/ExperimentLoader.hpp"
#include "algorithm-rt/NodeCounting.hpp"
#include "simulation/WorldLoader.hpp"
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
		World *world;
		std::string type;
		
		LOG(INFO) << "Loading config: " << p_file;
		root = YAML::LoadFile(p_file);
		LOG(INFO) << "-- file found";
		
		algorithm = root[ALGORITHM_NODE];
		LOG(INFO) <<  "-- algorithm config found";
		
		type = toLowerCase(algorithm[ALGORITHM_TYPE_NODE].as<std::string>());
		if(type == NODECOUNTING_TYPE)
			algorithmConfig.type = NODECOUNTING;
		else
			assert(false);
			
		algorithmConfig.obstacleAvoidDistance = algorithm[OBSTACLE_AVOID_DISTANCE_NODE].as<double>();
		algorithmConfig.obstacleMarkerDistance = algorithm[OBSTACLE_MARKER_DISTANCE_NODE].as<double>();
		
		world = WorldLoader::load(root);
		
		algorithmConfig.simulation = world->getSimulation();
		algorithmConfig.stock = world->getStock();
		
		Experiment *result = new Experiment();
		result->world_ = world;
		
		result->algorithms_.resize(world->getRobots().size());
		for(int i = 0; i < result->algorithms_.size(); ++i) {
			algorithmConfig.robot = world->getRobots()[i];
			
			switch(algorithmConfig.type) {
				case NODECOUNTING:
					result->algorithms_[i] = new NodeCounting(algorithmConfig);
					break;
			}
		}
		
		return result;
			
	}
}
