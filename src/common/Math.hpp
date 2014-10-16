#ifndef MAE_MATH_HPP
#define MAE_MATH_HPP

#include <cmath>

namespace mae
{
	inline double dmin(const double a, const double b)
	{ return a < b ? a : b; }
	inline double dmax(const double a, const double b)
	{ return a > b ? a : b; }
	
	inline float fmin(const float a, const float b)
	{ return a < b ? a : b; }
	inline float fmax(const float a, const float b)
	{ return a > b ? a : b; }
	
	inline int min(const int a, const int b)
	{ return a < b ? a : b; }
	inline int max(const int a, const int b)
	{ return a > b ? a : b; }
	
	inline double radianToDegree(const double p_radian)
	{ return p_radian * 180 / M_PI; }
	inline double degreeToRadian(const double p_degree)
	{ return p_degree * M_PI / 180;	}
	
	double normalizeRadian(const double p_radian);
	double normalizeDegree(const double p_degree);
	
	bool sameDouble(const double a, const double b, const double eps);
}

#endif
