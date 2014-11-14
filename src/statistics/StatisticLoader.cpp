#include <easylogging++.h>
#include "statistics/StatisticLoader.hpp"
#include "algorithm/ExperimentLoader.hpp"

#define STATISTIC_NODE "statistic"
#define FLOORPLAN_NAME "floorplan_name"
#define TILE_SIZE_NODE "tile_size"

namespace mae
{
    Statistic* StatisticLoader::load(const std::string &p_file)
    {
        YAML::Node root;
        LOG(INFO) << "Loading config: " << p_file;
		root = YAML::LoadFile(p_file);
		LOG(INFO) << "-- file found";
        return load(root);
    }

    Statistic* StatisticLoader::load(YAML::Node &p_root)
    {
        YAML::Node statisticNode;
        StatisticConfig config;

        statisticNode = p_root[STATISTIC_NODE];
        if(!statisticNode.IsDefined()) {
            LOG(INFO) << "-- no statistic config";
            return NULL;
        }
        LOG(INFO) << "-- statistic config found";
        
        assert(statisticNode[TILE_SIZE_NODE].IsSequence() && statisticNode[TILE_SIZE_NODE].size() == 2);
        config.tileSize.set(statisticNode[TILE_SIZE_NODE][0].as<double>(),
                            statisticNode[TILE_SIZE_NODE][1].as<double>());
        config.floorplanName = statisticNode[FLOORPLAN_NAME].as<std::string>();
        
        config.experiment = ExperimentLoader::load(p_root);
        
        return new Statistic(config);
    }

}
