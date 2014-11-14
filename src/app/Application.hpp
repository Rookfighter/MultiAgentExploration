#ifndef MAE_APPLICATION_HPP
#define MAE_APPLICATION_HPP

#include "algorithm/Experiment.hpp"
#include "statistics/Statistic.hpp"

namespace mae
{

	class Application
	{
	private:
		Experiment *experiment_;
        Statistic *statistic_;
        
        bool run_;
	public:
		Application(int argc, char** argv);
		~Application();
		
        void update();
		void run();

	};

}

#endif
