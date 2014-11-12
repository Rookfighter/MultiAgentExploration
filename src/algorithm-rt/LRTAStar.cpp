#include "algorithm-rt/LRTAStar.hpp"

namespace mae
{


    LRTAStar::LRTAStar(const AlgorithmConfig &p_config)
        :AntAlgorithm(p_config,
                      std::bind(&LRTAStar::updateValue, this, std::placeholders::_1, std::placeholders::_2),
                      std::bind(&LRTAStar::calcValue, this, std::placeholders::_1, std::placeholders::_2))
    {
    }

    LRTAStar::~LRTAStar()
    {
    }

    double LRTAStar::calcValue(Marker *p_current, Marker *p_next)
    {
        assert(p_next != NULL);

        double distance;
        if(p_current == NULL)
            distance = 1.0;
        else
            distance = (p_current->getPose().position - p_next->getPose().position).length();

        return  p_next->getValue() + distance;
    }

    void LRTAStar::updateValue(Marker *p_current, Marker *p_next)
    {
        assert(p_current != NULL);

        if(p_next == NULL)
            p_current->setValue(1.0);
        else
            p_current->setValue(calcValue(p_current, p_next));
    }


}
