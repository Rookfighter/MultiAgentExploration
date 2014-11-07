#ifndef MAE_SUBJECT_HPP
#define MAE_SUBJECT_HPP

#include <vector>
#include "utils/Observer.hpp"

namespace mae
{

	class Subject
	{
	private:
		std::vector<Observer*> observers_;
	public:
		Subject();
		~Subject();
		
		void addObserver(Observer *p_observer);
		void removeObserver(Observer *p_observer);
		void clearObserver();
		
		void notifyAll(void *p_data);

	};

}

#endif
