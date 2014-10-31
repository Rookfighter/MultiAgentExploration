#ifndef MAE_MATH_HPP
#define MAE_MATH_HPP

#include <cmath>

namespace mae
{
	inline double radianToDegree(const double p_radian)
	{ return p_radian * 180 / M_PI; }
	inline double degreeToRadian(const double p_degree)
	{ return p_degree * M_PI / 180;	}
	
	double normalizeRadian(const double p_radian);
	double normalizeDegree(const double p_degree);
	
	bool angleIsBetween(const double p_angle, const double p_begin, const double p_end);
	
	bool sameDouble(const double a, const double b, const double eps);
}

#endif
