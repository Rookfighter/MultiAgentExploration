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
        void copyFile(const std::string &p_source, const std::string &p_destination);
        void remove(const std::string &p_file);
    }

}

#endif
