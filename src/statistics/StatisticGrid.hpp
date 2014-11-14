#ifndef MAE_STATISTIC_GRID_HPP
#define MAE_STATISTIC_GRID_HPP

#include "statistics/StatisticTile.hpp"
#include "statistics/StatisticConfig.hpp"
#include "simulation/Simulation.hpp"
#include "utils/Vector2f.hpp"
#include "utils/Vector2i.hpp"

namespace mae
{

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
        
        void deleteGrid();
    public:
        StatisticGrid(const StatisticConfig &p_config);
        ~StatisticGrid();
        
        void setTileSize(const Vector2f &p_tileSize);
        void setWorldSize(const Vector2f &p_size);
        
        void visit(const Vector2f &p_position);
        void visit(const Vector2i &p_position);
        
        Vector2i getTilePosition(const Vector2f &p_position) const;
        const StatisticTile& getTile(const Vector2f &p_position) const;
        const StatisticTile& getTile(const Vector2i &p_position) const;
        
        double getMeanVisitCount() const;
        Stg::usec_t getMeanTimeBetweenVisits() const;
        double getCoverage() const;
        
        void update();
        
    };

}

#endif
