#ifndef MAE_APPLICATION_HPP
#define MAE_APPLICATION_HPP

#include "algorithm/Experiment.hpp"

namespace mae
{

	class Application
	{
	private:
		Experiment *experiment_;
	public:
		Application(int argc, char** argv);
		~Application();
		
		void run();

	};

}

#endif
