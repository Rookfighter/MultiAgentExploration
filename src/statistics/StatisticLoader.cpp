#include <easylogging++.h>
#include "statistics/StatisticLoader.hpp"
#include "algorithm/ExperimentLoader.hpp"
#include "utils/YamlNode.hpp"

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
        YAML::Node statisticNode, coverageEventNode, algorithmNode;
        StatisticConfig config;

        statisticNode = p_root[YamlNode::statistic];
        if(!statisticNode.IsDefined()) {
            LOG(INFO) << "-- no statistic config";
            return NULL;
        }
        LOG(INFO) << "-- statistic config found";

        assert(statisticNode[YamlNode::tileSize].IsSequence() && statisticNode[YamlNode::tileSize].size() == 2);
        config.tileSize.set(statisticNode[YamlNode::tileSize][0].as<double>(),
                            statisticNode[YamlNode::tileSize][1].as<double>());
        config.floorplanName = statisticNode[YamlNode::floorplanName].as<std::string>();
        
        algorithmNode = p_root[YamlNode::algorithm];
        assert(algorithmNode.IsDefined());
        config.algorithmType = algorithmNode[YamlNode::type].as<std::string>();

        coverageEventNode = statisticNode[YamlNode::coverageEvents];
        if(coverageEventNode.IsDefined() &&
                coverageEventNode.IsSequence() &&
                coverageEventNode.size() > 0) {
            config.coverageEvents.resize(coverageEventNode.size());
            
            for(unsigned int i = 0; i < coverageEventNode.size(); ++i)
                config.coverageEvents[i] = coverageEventNode[i].as<double>();
        } else {
            LOG(INFO) << "-- no coverage events found";
        }
        

        config.experiment = ExperimentLoader::load(p_root);

        return new Statistic(config);
    }

}
