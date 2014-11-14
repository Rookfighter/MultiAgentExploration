#ifndef MAE_STATISTIC_HPP
#define MAE_STATISTIC_HPP

#include "algorithm/Experiment.hpp"
#include "statistics/StatisticGrid.hpp"
#include "statistics/StatisticConfig.hpp"

namespace mae
{

    class Statistic
    {
        friend class StatisticLoader;
    private:
        Experiment *experiment_;
        StatisticGrid statisticGrid_;
        
        Statistic(const StatisticConfig &p_config);
    public:
        ~Statistic();
        
        Experiment* getExperiment();
        
        void saveToDirectory(const std::string &p_directory);
        void printLog();
        
        void update();
    };

}

#endif
