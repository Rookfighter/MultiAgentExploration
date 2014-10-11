#include "common/Math.hpp"

namespace mae
{	

	double normalizeRadian(const double p_radian)
	{
		double result = p_radian;
		while(result > 2 * M_PI)
			result -= 2 * M_PI;
		while(result < 0)
			result += 2 * M_PI;

		return result;
	}

	double normalizeDegree(const double p_degree)
	{
		double result = p_degree;
		while(result > 360)
			result -= 360;
		while(result < 0)
			result += 360;

		return result;
	}


}
