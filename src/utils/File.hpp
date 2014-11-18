#ifndef MAE_FILE_HPP
#define MAE_FILE_HPP

#include <string>

namespace mae
{

    namespace File
    {
        std::string cutTrailingSlash(const std::string &p_directory);
        void mkdirRec(const std::string &p_directory);
        bool exists(const std::string &p_directory);
    }

}

#endif
