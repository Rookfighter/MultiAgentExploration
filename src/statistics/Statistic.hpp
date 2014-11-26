#ifndef MAE_STATISTIC_HPP
#define MAE_STATISTIC_HPP

#include "simulation/Simulation.hpp"
#include "statistics/StatisticGrid.hpp"
#include "statistics/StatisticConfig.hpp"

namespace mae
{

    class Statistic
    {
        friend class StatisticLoader;
    private:
        Simulation *simulation_;
        StatisticGrid statisticGrid_;
        std::string saveDirectory_;
        std::string algorithmType_;

        void updateSaveDirectory(const std::string &p_directory);
        std::string getCurrentTime();
        void saveVisits();
        void saveTimeBetweenVisits();
        void saveCoverage();
        void saveExperimentConfig();
        void saveLogFile();

        Statistic(const StatisticConfig &p_config);
    public:
        ~Statistic();

        Simulation* getSimulation();

        const StatisticGrid& getStatisticGrid() const;

        void saveToDirectory(const std::string &p_directory);
        void printLog();

        void update();
    };

}

#endif
