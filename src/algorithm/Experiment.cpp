#include <easylogging++.h>
#include "algorithm/Experiment.hpp"

namespace mae
{
    Experiment::Experiment(const ExperimentConfig &p_config)
        :simulation_(p_config.simulation),
         statistic_(p_config.statistic),
         algorithms_(p_config.algorithms),
         experimentTermination_(p_config),
         algorithmWatch_(),
         robotCrashed_(algorithms_.size())
    {
        for(unsigned int i = 0; i < robotCrashed_.size(); ++i)
            robotCrashed_[i] = false;
    }

    Experiment::~Experiment()
    {
        for(Algorithm* algo: algorithms_)
            if(algo != NULL)
                delete algo;
        algorithms_.clear();

        if(statistic_ != NULL)
                delete statistic_;

        if(simulation_ != NULL)
            delete simulation_;
    }

    void Experiment::update()
    {
        if(terminated())
            return;

        for(unsigned int i = 0; i < robotCrashed_.size(); ++i) {
            if(!robotCrashed_[i] && simulation_->getRobots()[i]->collidedWithObstacle())
                LOG(WARNING) << "robot '" << simulation_->getRobots()[i]->getName() << "' collided with obstacle!";
            robotCrashed_[i] = simulation_->getRobots()[i]->collidedWithObstacle();
        }

        algorithmWatch_.start();
        for(Algorithm *algo : algorithms_)
            algo->update();

        algorithmWatch_.stop();

        /*LOG(DEBUG) << "Update: " << updateWatch_.getLastMsec() << "ms (" << updateWatch_.getWorstMsec() << "ms) "<<
                           "Algorithm: " << algorithmWatch_.getLastMsec() << "ms (" << algorithmWatch_.getWorstMsec() << "ms)";*/

        if(statistic_ != NULL)
            statistic_->update();
    }

    bool Experiment::terminated() const
    {
        return experimentTermination_.terminated();
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

    Statistic* Experiment::getStatistic()
    {
        return statistic_;
    }

}
