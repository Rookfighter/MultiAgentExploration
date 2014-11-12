#ifndef MAE_ANT_ALGORITHM_HPP
#define MAE_ANT_ALGORITHM_HPP

#include "algorithm/Algorithm.hpp"
#include "algorithm/AlgorithmConfig.hpp"
#include "algorithm-rt/AntState.hpp"

namespace mae
{

    class AntAlgorithm : public Algorithm
    {
    private:
        AntState *state_;
    public:
        AntAlgorithm(const AlgorithmConfig &p_config,
                     std::function<void(Marker*,Marker*)> p_updateValue,
                     std::function<double(Marker*,Marker*)> p_calcValue);
        virtual ~AntAlgorithm();

        void update();
    };

}

#endif
