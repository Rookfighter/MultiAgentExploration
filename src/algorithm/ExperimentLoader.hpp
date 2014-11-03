#ifndef MAE_EXPERIMENT_LOADER_HPP
#define MAE_EXPERIMENT_LOADER_HPP

#include "algorithm/Experiment.hpp"

namespace mae
{

	class ExperimentLoader
	{
	public:
		ExperimentLoader();
		~ExperimentLoader();

		static Experiment* load(const std::string &p_file);
	};

}

#endif
