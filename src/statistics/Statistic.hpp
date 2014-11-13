#ifndef MAE_STATISTIC_HPP
#define MAE_STATISTIC_HPP

#include "algorithm/Experiment.hpp"
#include "statistics/StatisticGrid.hpp"
#include "statistics/StatisticConfig.hpp"

namespace mae
{

    class Statistic
    {
    private:
        Experiment *experiment_;
        StatisticGrid statisticGrid_;
    public:
        Statistic(const StatisticConfig &p_config);
        ~Statistic();
        
        void saveToDirectory(const std::string &p_directory);
        void printLog();
        
        void update();
    };

}

#endif
