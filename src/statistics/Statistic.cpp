#include <easylogging++.h>
#include <ctime>
#include <sstream>
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include "statistics/Statistic.hpp"

#define VISITS_FILE "visits.txt"
#define MEAN_VISITS_FILE "mean-visits.txt"
#define TILE_MEAN_TIME_BEWTEEN_VISITS_FILE "tile-mean-time-between-visits.txt"
#define GRID_MEAN_TIME_BEWTEEN_VISITS_FILE "grid-mean-time-between-visits.txt"
#define COVERAGE_EVENTS_FILE "coverage-events.txt"
#define FINAL_COVERAGE_FILE "final-coverage.txt"

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
        std::string directory = p_directory;
        if(directory.back() == '/')
            directory.erase(directory.end());
        int ret;
        
        LOG(INFO) << "Saving statistics to \"" << directory << "\"";
        
        updateSaveDirectory(directory);
        LOG(INFO) << "-- creating directory \"" << saveDirectory_;
        ret = mkdir(saveDirectory_.c_str(), 777);
        if(ret != 0 && ret != EEXIST)
            throw std::logic_error(strerror(errno));
            
        LOG(INFO) << "-- saving visits";
        saveVisits();
        LOG(INFO) << "-- saving timeBetweenVisits";
        saveTimeBetweenVisits();
        LOG(INFO) << "-- saving coverage";
        saveCoverage();
    }

    void Statistic::updateSaveDirectory(const std::string &p_directory)
    {
        std::stringstream ss;
        ss << p_directory << "/" << algorithmType_ << "/" << getCurrentTime();
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
        ss << saveDirectory_ << "/" << VISITS_FILE;
        
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
        ss << saveDirectory_ << "/" << MEAN_VISITS_FILE;
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
        ss << saveDirectory_ << "/" << TILE_MEAN_TIME_BEWTEEN_VISITS_FILE;
        file.open(ss.str());
        file << "# shows meanTimeBetweenVisits of each tile" << std::endl;
        file << "# [x y meanTimeBetweenVisits(usec)]";
        for(int x = 0; x < statisticGrid_.getGridSize().x; ++x)
            for(int y = 0; y < statisticGrid_.getGridSize().y; ++y)
                file << std::endl << x << " " << y << " " << statisticGrid_.getTile(Vector2i(x,y)).getMeanTimeBetweenVisits();
        file.close();
        
        // save mean meanTimeBetweenVisits of whole grid
        ss.str(std::string());
        ss << saveDirectory_ << "/" << GRID_MEAN_TIME_BEWTEEN_VISITS_FILE;
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
        file << "# [timeStamp(usec) coverage]";
        for(const CoverageTime &time : statisticGrid_.getCoverageTimes()) {
            if(time.reached)
                file << std::endl << time.timeStamp << time.coverage;
        }
        file.close();
        
        // save final coverage of whole grid
        ss.str(std::string());
        ss << saveDirectory_ << "/" << FINAL_COVERAGE_FILE;
        file.open(ss.str());
        file << "# shows final coverage" << std::endl;
        file << "# [timeStamp(usec) coverage]";
        file << std::endl << experiment_->getSimulation()->getWorld()->SimTimeNow() << " " << statisticGrid_.getCoverage();
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
