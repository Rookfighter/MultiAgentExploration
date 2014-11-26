#ifndef MAE_CONVERT_HPP
#define MAE_CONVERT_HPP

#include <string>
#include <algorithm>
#include <stage.hh>

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

    inline void replaceSubstring(std::string &p_string, const std::string &p_find, const std::string &p_replacement)
    {
        for(size_t pos = 0; ; pos += p_replacement.length()) {
            // Locate the substring to replace
            pos = p_string.find(p_find, pos);
            if(pos == std::string::npos) break;
            // Replace by erasing and inserting
            p_string.erase(pos, p_find.length());
            p_string.insert(pos, p_replacement);
        }
    }

    inline int usecToMin(const Stg::usec_t p_usec)
    {
        return p_usec / 60000000;
    }
}

#endif
