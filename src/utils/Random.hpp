#ifndef MAE_RANDOM_HPP
#define MAE_RANDOM_HPP

namespace mae
{

	class Random
	{
	private:
		static bool initialized_;

		static void init();
		Random() { }
        ~Random() { }
	public:


		static int nextInt();
		static int nextInt(const int p_range);

		static float nextFloat();

		static bool nextBool();

	};

}

#endif
