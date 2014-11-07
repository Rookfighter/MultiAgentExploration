#include "utils/Math.hpp"

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
	
	void createRegularPolygon(const double p_radius, Vector2 *p_corners, const int p_cornerCount)
	{
		for(int i = 0; i < p_cornerCount; ++i) {
			p_corners[i].x = p_radius * cos((2 * M_PI * i) / p_cornerCount);
			p_corners[i].y = p_radius * sin((2 * M_PI * i) / p_cornerCount);
		}
	}


}
