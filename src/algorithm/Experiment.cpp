#include <easylogging++.h>
#include "algorithm/Experiment.hpp"

namespace mae
{

	Experiment::Experiment()
		:world_(NULL), algorithms_()
	{
	}

	Experiment::~Experiment()
	{
		for(Algorithm* algo: algorithms_)
			delete algo;
		algorithms_.clear();
		if(world_ != NULL)
			delete world_;
	}

	void Experiment::step()
	{
		updateWatch_.start();
		world_->update();
		updateWatch_.stop();

		algorithmWatch_.start();
		for(Algorithm *algo : algorithms_)
			algo->step();
		algorithmWatch_.stop();

		/*LOG(DEBUG) << "Update: " << updateWatch_.getLastMsec() << "ms (" << updateWatch_.getWorstMsec() << "ms) "<<
		           "Algorithm: " << algorithmWatch_.getLastMsec() << "ms (" << algorithmWatch_.getWorstMsec() << "ms)";*/
	}

	Algorithm* Experiment::getAlgorithmOf(const std::string &p_robotName)
	{
		for(int i = 0; i < world_->getRobots().size(); ++i) {
			if(world_->getRobots()[i]->getName() == p_robotName)
				return algorithms_[i];
		}

		return NULL;
	}

	std::vector<Algorithm*> Experiment::getAlgorithms()
	{
		return algorithms_;
	}

	World* Experiment::getWorld()
	{
		return world_;
	}
}
