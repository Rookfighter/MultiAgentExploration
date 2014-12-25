#include <easylogging++.h>
#include <ctime>
#include <sstream>
#include <fstream>
#include "statistics/Statistic.hpp"
#include "io/File.hpp"
#include "io/FileNames.hpp"

namespace mae
{

    Statistic::Statistic(const StatisticConfig &p_config)
        :simulation_(p_config.simulation),
         statisticGrid_(p_config),
         algorithmType_(p_config.algorithmType),
         worldType_(p_config.worldType)
    {
        LOG(INFO) << "Initialized Statistic";
    }

    Statistic::~Statistic()
    {
    }

    Simulation* Statistic::getSimulation()
    {
        return simulation_;
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
        LOG(INFO) << "-- saving log";
        saveLogFile();
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
        ss << saveDirectory_ << "/" << FileNames::numberOfVisitsFile;

        // save all visitCounts of each tile
        std::ofstream file;
        file.open(ss.str());
        file << "# shows numberOfVisits of each tile" << std::endl;
        file << "# [x y numberOfVisits]";
        for(int x = 0; x < statisticGrid_.getGridSize().x; ++x)
            for(int y = 0; y < statisticGrid_.getGridSize().y; ++y)
                file << std::endl << x << " " << y << " " << statisticGrid_.getTile(Vector2i(x,y)).getVisitCount();
        file.close();

        // save meanNumberOfVistsEvents of whole grid
        ss.str(std::string());
        ss << saveDirectory_ << "/" << FileNames::meanNumberOfVisitsEventsFile;
        file.open(ss.str());
        file << "# shows mean numberOfVisits of grid" << std::endl;
        file << "# [coverage numberOfVisits]";
        for(const CoverageTime &event : statisticGrid_.getCoverageEvents()) {
            if(event.reached)
                file << std::endl << event.coverage << " " << event.meanVisits;
        }
        file.close();
    }

    void Statistic::saveTimeBetweenVisits()
    {
        std::stringstream ss;
        std::ofstream file;

        // save all meanTimeBetweenVisits of each tile
        ss << saveDirectory_ << "/" << FileNames::meanTimeBetweenVisitsFile;
        file.open(ss.str());
        file << "# shows meanTimeBetweenVisits of each tile" << std::endl;
        file << "# [x y meanTimeBetweenVisits(usec)]";
        for(int x = 0; x < statisticGrid_.getGridSize().x; ++x)
            for(int y = 0; y < statisticGrid_.getGridSize().y; ++y)
                file << std::endl << x << " " << y << " " << statisticGrid_.getTile(Vector2i(x,y)).getMeanTimeBetweenVisits();
        file.close();

        // save mean meanTimeBetweenVisits of whole grid
        ss.str(std::string());
        ss << saveDirectory_ << "/" << FileNames::timeBetweenVisitsEventsFile;
        file.open(ss.str());
        file << "# shows mean TimeBetweenVisits of grid" << std::endl;
        file << "# [coverage meanTimeBetweenVisits(usec)]";
        for(const CoverageTime &event : statisticGrid_.getCoverageEvents()) {
            if(event.reached)
                file << std::endl << event.coverage << " " << event.meanTimeBetweenVisits;
        }
        file.close();
    }

    void Statistic::saveCoverage()
    {
        std::stringstream ss;
        std::ofstream file;

        // save all coverageEvents that occurred
        ss << saveDirectory_ << "/" << FileNames::coverageEventsFile;
        file.open(ss.str());
        file << "# shows coverageEvents" << std::endl;
        file << "# [coverage timeStamp(usec)]";
        for(const CoverageTime &time : statisticGrid_.getCoverageEvents()) {
            if(time.reached)
                file << std::endl << time.coverage << " " << time.timeStamp;
        }
        file.close();

        // save all timeEvents that occurred
        ss.str(std::string());
        ss << saveDirectory_ << "/" << FileNames::timeEventsFile;
        file.open(ss.str());
        file << "# shows timeEvents" << std::endl;
        file << "# [coverage timeStamp(usec)]";
        for(const CoverageTime &time : statisticGrid_.getTimeEvents()) {
            if(time.reached)
                file << std::endl << time.coverage << " " << time.timeStamp;
        }
        file.close();

        // save final coverage of whole grid
        ss.str(std::string());
        ss << saveDirectory_ << "/" << FileNames::finalCoverageFile;
        file.open(ss.str());
        file << "# shows final coverage" << std::endl;
        file << "# [coverage timeStamp(usec)]" << std::endl;
        file << statisticGrid_.getCoverage() << " " << simulation_->getWorld()->SimTimeNow();
        file.close();
    }
    
    void Statistic::saveExperimentConfig()
    {
        std::stringstream ss;
        std::ofstream file;
        
        ss << saveDirectory_ << "/" << FileNames::experimentConfigFile;
        file.open(ss.str());
        file << "# shows configuration of the experiment" << std::endl;
        file << "# robotCount, worldType" << std::endl;
        file << simulation_->getRobots().size() << std::endl;
        file << worldType_;
        file.close();
    }

    void Statistic::saveLogFile()
    {
        std::stringstream ss;

        ss << saveDirectory_ << "/" << FileNames::logFile;
        std::string outfile = ss.str();
        File::copyFile(FileNames::logFilePath, outfile);
    }

    void Statistic::printLog()
    {

    }

    void Statistic::update()
    {
        statisticGrid_.update();
    }
}
