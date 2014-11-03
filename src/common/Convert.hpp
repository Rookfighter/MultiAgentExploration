#ifndef MAE_CONVERT_HPP
#define MAE_CONVERT_HPP

#include <string>
#include <algorithm>

namespace mae
{
	inline std::string boolToStr(bool p_bool)
	{
		return p_bool ? "true" : "false";
	}
	
	inline std::string toLowerCase(const std::string p_string)
	{
		std::string result = p_string;
		std::transform(result.begin(), result.end(), result.begin(), ::tolower);
		return result;
	}
}

#endif
