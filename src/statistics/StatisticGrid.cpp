#include <easylogging++.h>
#include "statistics/StatisticGrid.hpp"
#include "utils/Math.hpp"

namespace mae
{

    StatisticGrid::StatisticGrid(const StatisticConfig &p_config)
        :simulation_(p_config.experiment->getSimulation()),
         tileSize_(0,0),
         size_(0,0),
         floorplanName_(p_config.floorplanName),
         grid_(),
         previousTilePosition_(simulation_->getRobots().size())
         
    {
        Vector2f worldSize;
        Stg::Model *floorplan = simulation_->getWorld()->GetModel(floorplanName_);
        worldSize.set(floorplan->GetGeom().size.x, floorplan->GetGeom().size.y);
        
        setTileSize(p_config.tileSize);
        LOG(INFO) << "WorldSize: " << worldSize.str();
        setWorldSize(worldSize);
        LOG(INFO) << "GridSize: " << size_.str();
        
        for(unsigned int i = 0; i < previousTilePosition_.size(); ++i)
            previousTilePosition_[i] = getTilePosition(simulation_->getRobots()[i]->getAbsolutePose().position);
            
        LOG(INFO) << "Initialized StatisticGrid";
    }

    StatisticGrid::~StatisticGrid()
    {
    }

    void StatisticGrid::setTileSize(const Vector2f &p_tileSize)
    {
        tileSize_ = p_tileSize;
    }

    void StatisticGrid::setWorldSize(const Vector2f &p_size)
    {
        worldSize_ = p_size;
        size_.set(ceil(worldSize_.x / tileSize_.x),
                  ceil(worldSize_.y / tileSize_.y));

        grid_ .resize(size_.x);
        for(int x = 0; x < size_.x; ++x) {
            grid_[x].resize(size_.y);
            for(int y = 0; y < size_.y; ++y) {
                grid_[x][y].setWorld(simulation_->getWorld());
            }
        }
    }

    void StatisticGrid::visit(const Vector2f &p_position)
    {
        visit(getTilePosition(p_position));
    }

    void StatisticGrid::visit(const Vector2i &p_position)
    {
        grid_[p_position.x][p_position.y].visit();
    }

    Vector2i StatisticGrid::getTilePosition(const Vector2f &p_position) const
    {
        Vector2f position = p_position + (worldSize_ / 2);
        assert(position.x >= 0 && position.y >= 0);
        return Vector2i(floor(position.x / tileSize_.x),
                        floor(position.y / tileSize_.y));
    }

    const StatisticTile& StatisticGrid::getTile(const Vector2f &p_position) const
    {
        return getTile(getTilePosition(p_position));
    }

    const StatisticTile& StatisticGrid::getTile(const Vector2i &p_position) const
    {
        return grid_[p_position.x][p_position.y];
    }

    double StatisticGrid::getMeanVisitCount() const
    {
        double sum = 0;

        for(int x = 0; x < size_.x; ++x) {
            for(int y = 0; y < size_.y; ++y)
                sum += grid_[x][y].getVisitCount();
        }

        return sum / ((double)(size_.x * size_.y));
    }

    Stg::usec_t StatisticGrid::getMeanTimeBetweenVisits() const
    {
        Stg::usec_t sum = 0;

        for(int x = 0; x < size_.x; ++x) {
            for(int y = 0; y < size_.y; ++y)
                sum += grid_[x][y].getMeanTimeBetweenVisits();
        }

        return sum / (size_.x * size_.y);
    }

    double StatisticGrid::getCoverage() const
    {
        int covered = 0;
        for(int x = 0; x < size_.x; ++x) {
            for(int y = 0; y < size_.y; ++y) {
                if(grid_[x][y].getVisitCount() > 0)
                    covered++;
            }
        }

        return ((double) covered) / ((double)(size_.x * size_.y));
    }

    void StatisticGrid::update()
    {
        for(unsigned int i = 0; i < previousTilePosition_.size(); ++i) {
            Vector2i currentPosition = getTilePosition(simulation_->getRobots()[i]->getAbsolutePose().position);
            if(currentPosition != previousTilePosition_[i]) {
                visit(currentPosition);
                previousTilePosition_[i] = currentPosition;
            }
        }
    }
}
