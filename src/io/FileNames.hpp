#ifndef MAE_FILE_NAMES_HPP
#define MAE_FILE_NAMES_HPP

#include <string>

namespace mae
{
    namespace FileNames
    {
        const std::string logFile = "multi-agent-exploration.log";
        const std::string logFilePath = "../log/" + logFile;

        const std::string tileVisitsFile = "tile-visits.dat";
        const std::string meanGridVisitsFile = "mean-grid-visits.dat";
        const std::string meanTileTimeBetweenVisitsFile =
                "mean-tile-time-between-visits.dat";
        const std::string meanGridTimeBetweenVisitsFile =
                "mean-grid-time-between-visits.dat";
        const std::string coverageEventsFile = "coverage-events.dat";
        const std::string timeEventsFile = "time-events.dat";
        const std::string finalCoverageFile = "final-coverage.dat";
        const std::string experimentConfigFile = "experiment-config.dat";
    }
}

#endif
