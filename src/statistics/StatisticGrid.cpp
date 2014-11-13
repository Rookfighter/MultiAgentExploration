#include "statistics/StatisticGrid.hpp"
#include "utils/Math.hpp"

namespace mae
{

    StatisticGrid::StatisticGrid(Simulation *p_simulation)
        :simulation_(p_simulation),
         tileSize_(0,0),
         width_(0),
         height_(0),
         grid_()
    {

    }

    StatisticGrid::~StatisticGrid()
    {
    }

    void StatisticGrid::setTileSize(const Vector2 &p_tileSize)
    {
        tileSize_ = p_tileSize;
    }

    void StatisticGrid::setSize(const Vector2 &p_size)
    {
        setSize(ceil(p_size.x / tileSize_.x),
                ceil(p_size.y / tileSize_.y));

    }

    void StatisticGrid::setSize(const int p_width, const int p_height)
    {
        width_ = p_width;
        height_ = p_height;

        grid_ .resize(width_);
        for(int x = 0; x < width_; ++x) {
            grid_[x].resize(width_);
            for(int y = 0; y < height_; ++y) {
                grid_[x][y].setWorld(simulation_->getWorld());
            }
        }

    }

    void StatisticGrid::visit(const Vector2 p_position)
    {
        visit(floor(p_position.x / tileSize_.x),
              floor(p_position.y / tileSize_.y));
    }

    void StatisticGrid::visit(const int x, const int y)
    {
        grid_[x][y].visit();
    }
    
    const StatisticTile& StatisticGrid::getTile(const int x, const int y) const
    {
        return grid_[x][y];
    }

    double StatisticGrid::getMeanVisitCount() const
    {
        double sum = 0;

        for(int x = 0; x < width_; ++x) {
            for(int y = 0; y < height_; ++y)
                sum += grid_[x][y].getVisitCount();
        }

        return sum / ((double)(width_ * height_));
    }

    Stg::usec_t StatisticGrid::getMeanTimeBetweenVisits() const
    {
        Stg::usec_t sum = 0;

        for(int x = 0; x < width_; ++x) {
            for(int y = 0; y < height_; ++y)
                sum += grid_[x][y].getMeanTimeBetweenVisits();
        }

        return sum / (width_ * height_);
    }

    double StatisticGrid::getCoverage() const
    {
        int covered = 0;
        for(int x = 0; x < width_; ++x) {
            for(int y = 0; y < height_; ++y) {
                if(grid_[x][y].getVisitCount() > 0)
                    covered++;
            }
        }
        
        return ((double) covered) / ((double) (width_ * height_));
    }
    
    void StatisticGrid::update()
    {
    }
}
