#ifndef MAE_EXPERIMENT_LOADER_HPP
#define MAE_EXPERIMENT_LOADER_HPP

#include "algorithm/Experiment.hpp"

namespace mae
{

	class ExperimentLoader
	{
	private:
		ExperimentLoader();
		~ExperimentLoader();
	public:

		static Experiment* load(const std::string &p_file);
	};

}

#endif
