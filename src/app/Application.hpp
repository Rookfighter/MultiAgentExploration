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
        
        bool run_;
        volatile bool keepRunning_;
        
        int exitCode_;
        std::string configFile_;

        bool init();

        void saveStatistics();
        
        void loop();
        void loopNonGUI();
        void loopGUI();
	public:
		Application(int argc, char** argv);
		~Application();
		
        int update();
		int run();
        void stop();

        int getExitCode();

	};

}

#endif
