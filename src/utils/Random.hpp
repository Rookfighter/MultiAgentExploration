#ifndef MAE_RANDOM_HPP
#define MAE_RANDOM_HPP

namespace mae
{

	class Random
	{
	private:
		static bool initialized_;
	public:
		Random();
		~Random();

		int nextInt();
		int nextInt(const int p_range);

		float nextFloat();

		bool nextBool();

	};

}

#endif
