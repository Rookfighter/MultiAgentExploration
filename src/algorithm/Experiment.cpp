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
		world_->update();
		for(Algorithm *algo : algorithms_)
			algo->step();
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
