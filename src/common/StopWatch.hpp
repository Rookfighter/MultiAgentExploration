#ifndef MAE_STOP_WATCH_HPP
#define MAE_STOP_WATCH_HPP

#include <sys/time.h>
#include <string>

namespace mae
{

	class StopWatch
	{
	private:
		struct timeval begin_;
	
		struct timeval last_;
		struct timeval worst_;
		struct timeval best_;
	public:
		StopWatch();
		~StopWatch();
		
		void start();
		void stop();
		
		int getLastMsec() const;
		int getWorstMsec() const;
		int getBestMsec() const;
		
		std::string str() const;
	};

}

#endif
