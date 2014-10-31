#include "common/Math.hpp"

namespace mae
{	
	/* normalize given angle in [-pi;pi) */
	double normalizeRadian(const double p_radian)
	{
		double result = p_radian;
		while(result >= M_PI)
			result -= 2 * M_PI;
		while(result < -M_PI)
			result += 2 * M_PI;

		return result;
	}
	
	/* normalize given angle in [-180;180) */
	double normalizeDegree(const double p_degree)
	{
		double result = p_degree;
		while(result >= 180)
			result -= 360;
		while(result < -180)
			result += 360;

		return result;
	}
	
	bool angleIsBetween(const double p_angle, const double p_begin, const double p_end)
	{
		double angle = normalizeRadian(p_angle);
		double begin = normalizeRadian(p_begin);
		double end = normalizeRadian(p_end);
		
		// overflow
		if(end < begin) {
			return begin <= angle || angle <= end;
		} else {
			return begin <= angle && angle <= end;
		}
	}
	
	bool sameDouble(const double a, const double b, const double eps)
	{
		return fabs(a - b) <= eps;
	}


}
