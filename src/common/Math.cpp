#include "common/Math.hpp"

namespace mae
{	

	double normalizeRadian(const double p_radian)
	{
		double result = p_radian;
		while(result > M_PI)
			result -= 2 * M_PI;
		while(result < -M_PI)
			result += 2 * M_PI;

		return result;
	}

	double normalizeDegree(const double p_degree)
	{
		double result = p_degree;
		while(result > 180)
			result -= 360;
		while(result < -180)
			result += 360;

		return result;
	}
	
	bool sameDouble(const double a, const double b, const double eps)
	{
		return fabs(a - b) <= eps;
	}


}
