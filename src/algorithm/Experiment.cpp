#include <easylogging++.h>
#include "algorithm/Experiment.hpp"

namespace mae
{
    Experiment::Experiment()
        :simulation_(NULL),
         algorithms_()
    {
    }

    Experiment::~Experiment()
    {
        for(Algorithm* algo: algorithms_)
            delete algo;
        algorithms_.clear();
        if(simulation_ != NULL)
            delete simulation_;
    }

    void Experiment::update()
    {
        algorithmWatch_.start();
        for(Algorithm *algo : algorithms_)
            algo->update();

        algorithmWatch_.stop();

        /*LOG(DEBUG) << "Update: " << updateWatch_.getLastMsec() << "ms (" << updateWatch_.getWorstMsec() << "ms) "<<
                   "Algorithm: " << algorithmWatch_.getLastMsec() << "ms (" << algorithmWatch_.getWorstMsec() << "ms)";*/
    }

    Algorithm* Experiment::getAlgorithmOf(const std::string &p_robotName)
    {
        for(unsigned int i = 0; i < simulation_->getRobots().size(); ++i) {
            if(simulation_->getRobots()[i]->getName() == p_robotName)
                return algorithms_[i];
        }

        return NULL;
    }

    std::vector<Algorithm*> Experiment::getAlgorithms()
    {
        return algorithms_;
    }

    Simulation* Experiment::getSimulation()
    {
        return simulation_;
    }
}
