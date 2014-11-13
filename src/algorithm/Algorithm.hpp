#ifndef MAE_ALGORITHM_HPP
#define MAE_ALGORITHM_HPP

#include "algorithm/AlgorithmConfig.hpp"

namespace mae
{

    class Algorithm
    {
    protected:
        std::string type_;
    public:
        Algorithm(const AlgorithmConfig &p_config)
            :type_(p_config.type)
        { }
        virtual ~Algorithm()
        { }

        virtual void update() = 0;

        const std::string& getType() const {
            return type_;
        }
    };

}

#endif
