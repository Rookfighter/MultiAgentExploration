#ifndef MAE_STATISTIC_LOADER_HPP
#define MAE_STATISTIC_LOADER_HPP

#include <yaml-cpp/yaml.h>
#include "statistics/Statistic.hpp"

namespace mae
{
    class StatisticLoader
    {
    private:
        StatisticConfig config_;
        bool found_;
    public:
        StatisticLoader();
        ~StatisticLoader();

        void load(const std::string &p_file);
        void load(YAML::Node &p_root);

        Statistic* create(Simulation *p_simulation);
    };

}

#endif
