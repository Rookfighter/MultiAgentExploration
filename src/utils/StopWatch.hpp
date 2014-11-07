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
		
		bool newBestCase_;
		bool newWorstCase_;
	public:
		StopWatch();
		~StopWatch();
		
		void start();
		void stop();
		
		unsigned int getLastMsec() const;
		unsigned int getWorstMsec() const;
		unsigned int getBestMsec() const;
		
		unsigned long getLastUsec() const;
		unsigned long getWorstUsec() const;
		unsigned long getBestUsec() const;
		
		bool hasNewWorstCase() const;
		bool hasNewBestCase() const;
		
		std::string strMsec() const;
		std::string strUsec() const;
	};

}

#endif
