#include <cstddef>
#include <sstream>
#include "utils/StopWatch.hpp"
#include "utils/Math.hpp"

namespace mae
{
	StopWatch::StopWatch()
		:newBestCase_(false), newWorstCase_(false)
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

		last_ = getTimevalDiff(&begin_, &end);

		// check if it is new worst case
		newWorstCase_ = last_.tv_sec > worst_.tv_sec ||
		                (last_.tv_sec == worst_.tv_sec && last_.tv_usec > worst_.tv_usec);
		if(newWorstCase_)
			worst_ = last_;

		// check if it is new best case
		newBestCase_ = last_.tv_sec < best_.tv_sec ||
		               (last_.tv_sec == best_.tv_sec && last_.tv_usec < best_.tv_usec);
		if(newBestCase_)
			best_ = last_;
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

	bool StopWatch::hasNewWorstCase() const
	{
		return newWorstCase_;
	}

	       bool StopWatch::hasNewBestCase() const
	{
		return newBestCase_;
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
