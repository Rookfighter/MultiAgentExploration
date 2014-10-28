#include "common/Subject.hpp"

namespace mae
{

	Subject::Subject()
		:observers_(0)
	{
	}

	Subject::~Subject()
	{
	}

	void Subject::addObserver(Observer *p_observer)
	{
		observers_.push_back(p_observer);
	}

	void Subject::removeObserver(Observer *p_observer)
	{
		std::vector<Observer*>::iterator it;
		for(it = observers_.begin(); it != observers_.end(); ++it)
			if((*it) == p_observer)
				break;

		observers_.erase(it);
	}
	void Subject::clearObserver()
	{
		observers_.clear();
	}

	void Subject::notifyAll(void *p_data)
	{
		for(Observer *observer : observers_)
			observer->notify(p_data);
	}

}
