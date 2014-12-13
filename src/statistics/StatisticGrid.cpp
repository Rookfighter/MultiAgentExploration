#include <easylogging++.h>
#include "statistics/StatisticGrid.hpp"
#include "utils/Math.hpp"
#include "utils/Convert.hpp"

namespace mae
{

    StatisticGrid::StatisticGrid(const StatisticConfig &p_config)
        :simulation_(p_config.simulation),
         tileSize_(0,0),
         size_(0,0),
         floorplanName_(p_config.floorplanName),
         grid_(),
         previousTilePosition_(simulation_->getRobots().size()),
         coverageEvents_(),
		 timeEvents_(),
         coveredByObstacles(p_config.coveredByObstacles)
    {
        Vector2f worldSize;
        Stg::Model *floorplan = simulation_->getWorld()->GetModel(floorplanName_);
        worldSize.set(floorplan->GetGeom().size.x, floorplan->GetGeom().size.y);

        setTileSize(p_config.tileSize);
        LOG(INFO) << "WorldSize: " << worldSize.str();
        setWorldSize(worldSize);

        for(unsigned int i = 0; i < previousTilePosition_.size(); ++i) {
            previousTilePosition_[i] = getTilePosition(simulation_->getRobots()[i]->getAbsolutePose().position);
            visit(previousTilePosition_[i]);
        }
        
        for(double coverageEvent : p_config.coverageEvents)
            addCoverageEvent(coverageEvent);
        for(unsigned int timeEvent: p_config.timeEvents)
        	addTimeEvent(minToUsec(timeEvent));

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

    void StatisticGrid::addCoverageEvent(const double p_coverage)
    {
        CoverageTime time;
        time.coverage = p_coverage;
        time.reached = false;
        time.timeStamp = 0;
        coverageEvents_.push_back(time);
    }

    void StatisticGrid::addTimeEvent(const Stg::usec_t p_time)
    {
    	CoverageTime time;
    	time.coverage = 0;
    	time.reached = false;
    	time.timeStamp = p_time;
    	timeEvents_.push_back(time);
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
    
    const Vector2f& StatisticGrid::getWorldSize() const
    {
        return worldSize_;
    }
    
    const Vector2i& StatisticGrid::getGridSize() const
    {
        return size_;
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
        double result = ((double) covered) / ((1.0 - coveredByObstacles) * (double)(size_.x * size_.y));
        if(result > 1.0)
        	result = 1.0;
        return result;
    }

    const std::vector<CoverageTime>& StatisticGrid::getCoverageEvents() const
    {
        return coverageEvents_;
    }

    void StatisticGrid::update()
    {
        bool visitHappened = false;
        for(unsigned int i = 0; i < previousTilePosition_.size(); ++i) {
            Vector2i currentPosition = getTilePosition(simulation_->getRobots()[i]->getAbsolutePose().position);
            if(currentPosition != previousTilePosition_[i]) {
                visit(currentPosition);
                previousTilePosition_[i] = currentPosition;
                visitHappened = true;
            }
        }

        if(visitHappened)
            updateCoverageEvents();
        updateTimeEvents();
    }

    void StatisticGrid::updateCoverageEvents()
    {
        double currentCoverage = getCoverage();
        Stg::usec_t currentTime = simulation_->getWorld()->SimTimeNow();

        for(CoverageTime &time : coverageEvents_) {
            if(!time.reached && currentCoverage >= time.coverage) {
                time.reached = true;
                time.timeStamp = currentTime;
            }
        }
    }

    void StatisticGrid::updateTimeEvents()
    {
    	double currentCoverage = getCoverage();
    	Stg::usec_t currentTime = simulation_->getWorld()->SimTimeNow();

    	for(CoverageTime &time: timeEvents_) {
    		if(!time.reached && currentTime >= time.timeStamp) {
    			time.reached = true;
    			time.coverage = currentCoverage;
    		}
    	}
    }
}
