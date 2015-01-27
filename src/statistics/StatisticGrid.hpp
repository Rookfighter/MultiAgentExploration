#ifndef MAE_STATISTIC_GRID_HPP
#define MAE_STATISTIC_GRID_HPP

#include <array>
#include "statistics/StatisticTile.hpp"
#include "statistics/StatisticConfig.hpp"
#include "simulation/Simulation.hpp"
#include "utils/Vector2f.hpp"
#include "utils/Vector2i.hpp"

namespace mae
{
    struct CoverageTime {
        double coverage;
        Stg::usec_t timeStamp;
        int meanTimeBetweenVisits; //msec
        int stdTimeBetweenVisits; // msec
        double meanVisits;
        double stdVisits;
        bool reached;
    };

    class StatisticGrid
    {
    private:
        Simulation *simulation_;
        Vector2f tileSize_;
        Vector2f worldSize_;
        Vector2i size_;
        std::string floorplanName_;

        std::vector<std::vector<StatisticTile>> grid_;
        std::vector<Vector2i> previousTilePosition_;

        std::vector<CoverageTime> coverageEvents_;
        std::vector<CoverageTime> timeEvents_;

        double coveredByObstacles;

        void updateCoverageEvents();
        void updateTimeEvents();
    public:
        StatisticGrid(const StatisticConfig &p_config);
        ~StatisticGrid();

        void setTileSize(const Vector2f &p_tileSize);
        void setWorldSize(const Vector2f &p_size);

        void visit(const Vector2f &p_position);
        void visit(const Vector2i &p_position);

        void addCoverageEvent(const double p_coverage);
        void addTimeEvent(const Stg::usec_t p_time);

        Vector2i getTilePosition(const Vector2f &p_position) const;
        const StatisticTile& getTile(const Vector2f &p_position) const;
        const StatisticTile& getTile(const Vector2i &p_position) const;
        const Vector2f& getWorldSize() const;
        const Vector2i& getGridSize() const;

        std::array<double, 2> getMeanVisitCount() const;
        std::array<int, 2> getMeanTimeBetweenVisits() const;
        const std::vector<CoverageTime>& getCoverageEvents() const;
        const std::vector<CoverageTime>& getTimeEvents() const;
        double getCoverage() const;

        void update();
    };

}

#endif
