#ifndef MAE_ALGORITHM_HPP
#define MAE_ALGORITHM_HPP

namespace mae
{

	class Algorithm
	{
	public:
		Algorithm() {
		}
		virtual ~Algorithm() {
		}
		
		virtual void step() = 0;
	};

}

#endif // MAE_ALGORITHM_HPP
