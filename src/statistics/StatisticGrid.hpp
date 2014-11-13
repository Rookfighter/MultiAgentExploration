#ifndef MAE_STATISTIC_GRID_HPP
#define MAE_STATISTIC_GRID_HPP

#include "utils/Odometry.hpp"
#include "statistics/StatisticTile.hpp"

namespace mae
{

    class StatisticGrid
    {
    private:
        Stg::World *world_;
        Vector2 tileSize_;
        
        int width_;
        int height_;
        std::vector<std::vector<StatisticTile>> grid_;
        
        void deleteGrid();
    public:
        StatisticGrid(Stg::World *p_world);
        ~StatisticGrid();
        
        void setTileSize(const Vector2 &p_tileSize);
        void setSize(const Vector2 &size);
        void setSize(const int p_width, const int p_height);
        
        void visit(const Vector2 p_position);
        void visit(const int x, const int y);
        
        const StatisticTile& getTile(const int x, const int y) const;
        
        double getMeanVisitCount() const;
        Stg::usec_t getMeanTimeBetweenVisits() const;
        double getCoverage() const;
        
    };

}

#endif
