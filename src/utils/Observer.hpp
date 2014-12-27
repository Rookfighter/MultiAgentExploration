#ifndef MAE_OBSERVER_HPP
#define MAE_OBSERVER_HPP

namespace mae
{

	class Observer
	{
	public:
		Observer();
		virtual ~Observer();
		
		virtual void notify(void *p_data) = 0;
	};

}

#endif
