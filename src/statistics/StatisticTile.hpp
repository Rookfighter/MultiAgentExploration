#ifndef MAE_STATISTIC_TILE_HPP
#define MAE_STATISTIC_TILE_HPP

#include <stage.hh>
#include <list>
#include <vector>

namespace mae
{
    struct VisitInfo {
        Stg::usec_t timeStamp;
    };
    
    class StatisticTile
    {
    private:
        Stg::World *world_;
        std::list<VisitInfo> visits_;
        
    public:
        StatisticTile();
        ~StatisticTile();
        
        void setWorld(Stg::World *p_world);
        void visit();
        
        int getVisitCount() const;
        std::vector<Stg::usec_t> getTimeBetweenVisits() const;
        Stg::usec_t getMeanTimeBetweenVisits() const;
        const std::list<VisitInfo>& getVisits() const;
        bool hasBeenVisited() const;
        
    };

}

#endif
