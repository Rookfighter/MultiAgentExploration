#include "statistics/StatisticTile.hpp"

namespace mae
{

    StatisticTile::StatisticTile()
        :world_(NULL),
         visits_()
    {

    }

    StatisticTile::~StatisticTile()
    {

    }

    void StatisticTile::setWorld(Stg::World *p_world)
    {
        world_ = p_world;
    }

    void StatisticTile::visit()
    {
        VisitInfo info;
        info.timeStamp = world_->SimTimeNow();

        visits_.push_back(info);
    }

    int StatisticTile::getVisitCount() const
    {
        return visits_.size();
    }
    
    std::vector<Stg::usec_t> StatisticTile::getTimeBetweenVisits() const
    {
        if(visits_.empty())
            return std::vector<Stg::usec_t>();
            
        std::vector<Stg::usec_t> result(visits_.size() - 1);
        VisitInfo last;
        int current = -1;

        for(VisitInfo visit : visits_) {
            if(current >= 0)
                result[current] = (visit.timeStamp - last.timeStamp);

            last = visit;
            current++;
        }
        
        return result;
    }
    
    Stg::usec_t StatisticTile::getMeanTimeBetweenVisits() const
    {
        std::vector<Stg::usec_t> timeBetweenVisits = getTimeBetweenVisits();
        Stg::usec_t sum = 0;

        for(Stg::usec_t timeDiff : timeBetweenVisits)
            sum += timeDiff;
        
        return sum / ((double) timeBetweenVisits.size());
    }

    const std::list<VisitInfo>& StatisticTile::getVisits() const
    {
        return visits_;
    }

    bool StatisticTile::hasBeenVisited() const
    {
        return !visits_.empty();
    }

}
