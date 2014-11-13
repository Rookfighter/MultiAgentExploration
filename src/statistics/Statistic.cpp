#include "statistics/Statistic.hpp"

namespace mae
{

    Statistic::Statistic(const StatisticConfig &p_config)
    :experiment_(p_config.experiment),
    statisticGrid_(experiment_->getSimulation())
    {
        statisticGrid_.setTileSize(p_config.tileSize);
        statisticGrid_.setSize(p_config.gridSize);
    }

    Statistic::~Statistic()
    {
    }
    
    void Statistic::saveToDirectory(const std::string &p_directory)
    {
        
    }
    
    void Statistic::printLog()
    {
        
    }

    void update()
    {
        
    }
}
