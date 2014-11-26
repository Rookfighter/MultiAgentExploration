#include "../io/StatisticLoader.hpp"

#include <easylogging++.h>
#include "../io/ExperimentLoader.hpp"
#include "../io/YamlNode.hpp"

namespace mae
{
    StatisticLoader::StatisticLoader()
    :found_(false)
    {
    }

    StatisticLoader::~StatisticLoader()
    {
    }

    void StatisticLoader::load(const std::string &p_file)
    {
        YAML::Node root;
        LOG(INFO) << "Loading config: " << p_file;
        root = YAML::LoadFile(p_file);
        LOG(INFO) << "-- file found";
        load(root);
    }

    void StatisticLoader::load(YAML::Node &p_root)
    {
        YAML::Node statisticNode, coverageEventNode, algorithmNode;

        statisticNode = p_root[YamlNode::statistic];
        found_ = true;
        if(!statisticNode.IsDefined()) {
            LOG(INFO) << "-- statistic not found config";
            found_ = false;
            return;
        }
        LOG(INFO) << "-- statistic config found";

        assert(statisticNode[YamlNode::tileSize].IsSequence() && statisticNode[YamlNode::tileSize].size() == 2);
        config_.tileSize.set(statisticNode[YamlNode::tileSize][0].as<double>(),
                            statisticNode[YamlNode::tileSize][1].as<double>());
        config_.floorplanName = statisticNode[YamlNode::floorplanName].as<std::string>();
        
        algorithmNode = p_root[YamlNode::algorithm];
        assert(algorithmNode.IsDefined());
        config_.algorithmType = algorithmNode[YamlNode::type].as<std::string>();

        coverageEventNode = statisticNode[YamlNode::coverageEvents];
        if(coverageEventNode.IsDefined() &&
                coverageEventNode.IsSequence() &&
                coverageEventNode.size() > 0) {
            config_.coverageEvents.resize(coverageEventNode.size());
            
            for(unsigned int i = 0; i < coverageEventNode.size(); ++i)
                config_.coverageEvents[i] = coverageEventNode[i].as<double>();
        } else {
            LOG(INFO) << "-- no coverage events found";
        }
    }

    Statistic* StatisticLoader::create(Simulation* p_simulation)
    {
        if(!found_)
            return NULL;

        config_.simulation = p_simulation;
        return new Statistic(config_);
    }
}
