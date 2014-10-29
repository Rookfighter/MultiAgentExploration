#include <cstddef>
#include <sstream>
#include "common/StopWatch.hpp"

#define MICROSECONDS_PER_SECOND 1000000

namespace mae
{
	static unsigned int msecOf(const struct timeval *p_tv)
	{
		return p_tv->tv_sec * 1000 + p_tv->tv_usec / 1000;
	}
	
	static unsigned long usecOf(const struct timeval *p_tv)
	{
		return p_tv->tv_sec * 1000000 + p_tv->tv_usec;
	}

	struct timeval get_time_diff(const struct timeval *begin, const struct timeval *end)
	{
		struct timeval tmp;

		tmp.tv_sec = end->tv_sec - begin->tv_sec;
		tmp.tv_usec = end->tv_usec - begin->tv_usec;

		if(tmp.tv_usec < 0) {
			tmp.tv_sec--;
			tmp.tv_usec = MICROSECONDS_PER_SECOND + tmp.tv_usec;
		}

		return tmp;
	}

	StopWatch::StopWatch()
	{
		best_.tv_sec = 999;
		best_.tv_usec = 999;
		
		worst_.tv_sec = 0;
		worst_.tv_usec = 0;
	}

	StopWatch::~StopWatch()
	{
	}

	void StopWatch::start()
	{
		gettimeofday(&begin_, NULL);
	}
	void StopWatch::stop()
	{
		struct timeval end;
		gettimeofday(&end, NULL);
		
		last_ = get_time_diff(&begin_, &end);
		
		// check if it is new worst case
		if(last_.tv_sec > worst_.tv_sec)
			worst_ = last_;
		else if(last_.tv_sec == worst_.tv_sec) {
			if(last_.tv_usec > worst_.tv_usec)
				worst_ = last_;
		}
		
		// check if it is new best case
		if(last_.tv_sec < best_.tv_sec)
			best_ = last_;
		else if(last_.tv_sec == best_.tv_sec) {
			if(last_.tv_usec < best_.tv_usec)
				best_ = last_;
		}
	}

	unsigned int StopWatch::getLastMsec() const
	{
		return msecOf(&last_);
	}

	unsigned int StopWatch::getWorstMsec() const
	{
		return msecOf(&worst_);
	}

	unsigned int StopWatch::getBestMsec() const
	{
		return msecOf(&best_);
	}
	
	unsigned long StopWatch::getLastUsec() const
	{
		return usecOf(&last_);
	}
	unsigned long StopWatch::getWorstUsec() const
	{
		return usecOf(&worst_);
	}
	
	unsigned long StopWatch::getBestUsec() const
	{
		return usecOf(&best_);
	}
	
	std::string StopWatch::strMsec() const
	{
		std::stringstream ss;
		
		ss << "Last=" << getLastMsec() << "ms Best=" << getBestMsec() << "ms Worst=" << getWorstMsec() << "ms";
		return ss.str();
	}
	
	std::string StopWatch::strUsec() const
	{
		std::stringstream ss;
		
		ss << "Last=" << getLastUsec() << "us Best=" << getBestUsec() << "us Worst=" << getWorstUsec() << "us";
		return ss.str();
	}

}
