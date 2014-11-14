#ifndef MAE_STATISTIC_LOADER_HPP
#define MAE_STATISTIC_LOADER_HPP

#include <yaml-cpp/yaml.h>
#include "statistics/Statistic.hpp"

namespace mae
{

    class StatisticLoader
    {
    private:
        StatisticLoader() { }
        ~StatisticLoader() { }
    public:
        static Statistic* load(const std::string &p_file, Experiment *p_experiment);
        static Statistic* load(YAML::Node &p_root, Experiment *p_experiment);
    };

}

#endif
