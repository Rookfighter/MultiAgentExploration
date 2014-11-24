#include <easylogging++.h>
#include <ctime>
#include <sstream>
#include <fstream>
#include "statistics/Statistic.hpp"
#include "utils/File.hpp"

#define TILE_VISITS_FILE "tile-visits.dat"
#define MEAN_GRID_VISITS_FILE "mean-grid-visits.dat"
#define MEAN_TILE_TIME_BEWTEEN_VISITS_FILE "mean-tile-time-between-visits.dat"
#define MEAN_GRID_TIME_BEWTEEN_VISITS_FILE "mean-grid-time-between-visits.dat"
#define COVERAGE_EVENTS_FILE "coverage-events.dat"
#define FINAL_COVERAGE_FILE "final-coverage.dat"
#define EXPERIMENT_CONFIG_FILE "experiment-config.dat"

namespace mae
{

    Statistic::Statistic(const StatisticConfig &p_config)
        :experiment_(p_config.experiment),
         statisticGrid_(p_config),
         algorithmType_(p_config.algorithmType)
    {
        LOG(INFO) << "Initialized Statistic";
    }

    Statistic::~Statistic()
    {
    }

    Experiment* Statistic::getExperiment()
    {
        return experiment_;
    }

    const StatisticGrid& Statistic::getStatisticGrid() const
    {
        return statisticGrid_;
    }

    void Statistic::saveToDirectory(const std::string &p_directory)
    {
        LOG(INFO) << "Saving statistics to \"" << p_directory << "\"";
        
        updateSaveDirectory(p_directory);
        
        LOG(INFO) << "-- creating directory \"" << p_directory << "\"";
        File::mkdirRec(saveDirectory_);

        LOG(INFO) << "-- saving visits";
        saveVisits();
        LOG(INFO) << "-- saving timeBetweenVisits";
        saveTimeBetweenVisits();
        LOG(INFO) << "-- saving coverage";
        saveCoverage();
        LOG(INFO) << "-- saving experiment config";
        saveExperimentConfig();
    }

    void Statistic::updateSaveDirectory(const std::string &p_directory)
    {
        std::stringstream ss;
        ss << File::cutTrailingSlash(p_directory) << "/" << algorithmType_ << "/" << getCurrentTime();
        saveDirectory_ = ss.str();
    }

    std::string Statistic::getCurrentTime()
    {
        time_t currentTime = time(NULL);
        struct tm localTime = *localtime(&currentTime);

        std::stringstream ss;
        ss << localTime.tm_year + 1900 << "-" <<
           localTime.tm_mon + 1 << "-" <<
           localTime.tm_wday + 1 << "-" <<
           localTime.tm_hour << "-" <<
           localTime.tm_min << "-" <<
           localTime.tm_sec;

        return ss.str();
    }

    void Statistic::saveVisits()
    {
        std::stringstream ss;
        ss << saveDirectory_ << "/" << TILE_VISITS_FILE;

        // save all visitCounts of each tile
        std::ofstream file;
        file.open(ss.str());
        file << "# shows visitCount of each tile" << std::endl;
        file << "# [x y visitCount]";
        for(int x = 0; x < statisticGrid_.getGridSize().x; ++x)
            for(int y = 0; y < statisticGrid_.getGridSize().y; ++y)
                file << std::endl << x << " " << y << " " << statisticGrid_.getTile(Vector2i(x,y)).getVisitCount();
        file.close();

        // save mean meanVisitCount of whole grid
        ss.str(std::string());
        ss << saveDirectory_ << "/" << MEAN_GRID_VISITS_FILE;
        file.open(ss.str());
        file << "# shows mean visitCount of whole map" << std::endl;
        file << "# [meanVisitCount]";
        file << std::endl << statisticGrid_.getMeanVisitCount();
        file.close();
    }

    void Statistic::saveTimeBetweenVisits()
    {
        std::stringstream ss;
        std::ofstream file;

        // save all meanTimeBetweenVisits of each tile
        ss << saveDirectory_ << "/" << MEAN_TILE_TIME_BEWTEEN_VISITS_FILE;
        file.open(ss.str());
        file << "# shows meanTimeBetweenVisits of each tile" << std::endl;
        file << "# [x y meanTimeBetweenVisits(usec)]";
        for(int x = 0; x < statisticGrid_.getGridSize().x; ++x)
            for(int y = 0; y < statisticGrid_.getGridSize().y; ++y)
                file << std::endl << x << " " << y << " " << statisticGrid_.getTile(Vector2i(x,y)).getMeanTimeBetweenVisits();
        file.close();

        // save mean meanTimeBetweenVisits of whole grid
        ss.str(std::string());
        ss << saveDirectory_ << "/" << MEAN_GRID_TIME_BEWTEEN_VISITS_FILE;
        file.open(ss.str());
        file << "# shows mean TimeBetweenVisits of whole map" << std::endl;
        file << "# [meanTimeBetweenVisits(usec)]";
        file << std::endl << statisticGrid_.getMeanTimeBetweenVisits();
        file.close();
    }

    void Statistic::saveCoverage()
    {
        std::stringstream ss;
        std::ofstream file;

        // save all coverageEvents that occurred
        ss << saveDirectory_ << "/" << COVERAGE_EVENTS_FILE;
        file.open(ss.str());
        file << "# shows coverageEvents" << std::endl;
        file << "# [coverage timeStamp(usec)]";
        for(const CoverageTime &time : statisticGrid_.getCoverageTimes()) {
            if(time.reached)
                file << std::endl << time.coverage << " " << time.timeStamp;
        }
        file.close();

        // save final coverage of whole grid
        ss.str(std::string());
        ss << saveDirectory_ << "/" << FINAL_COVERAGE_FILE;
        file.open(ss.str());
        file << "# shows final coverage" << std::endl;
        file << "# [coverage timeStamp(usec)]" << std::endl;
        file << statisticGrid_.getCoverage() << " " << experiment_->getSimulation()->getWorld()->SimTimeNow();
        file.close();
    }
    
    void Statistic::saveExperimentConfig()
    {
        std::stringstream ss;
        std::ofstream file;
        
        ss << saveDirectory_ << "/" << EXPERIMENT_CONFIG_FILE;
        file.open(ss.str());
        file << "# shows configuration of the experiment" << std::endl;
        file << "# robotCount" << std::endl;
        file << experiment_->getSimulation()->getRobots().size();
        file.close();
    }

    void Statistic::printLog()
    {

    }

    void Statistic::update()
    {
        statisticGrid_.update();
    }
}
