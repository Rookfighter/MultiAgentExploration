#include "io/File.hpp"
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <sstream>
#include <cstring>
#include <stdexcept>

namespace mae
{

    namespace File
    {
        std::string cutTrailingSlash(const std::string &p_directory)
        {
            std::string result = p_directory;
            if(result.back() == '/')
                result.erase(result.end());
                
            return result;
        }
    
        void mkdirRec(const std::string &p_directory)
        {
            char tmp[256];
            char *p = NULL;

            snprintf(tmp, sizeof(tmp),"%s",cutTrailingSlash(p_directory).c_str());
            for(p = tmp + 1; *p; p++) {
                if(*p == '/') {
                    *p = 0;
                    if(!exists(std::string(tmp))) {
                        if(mkdir(tmp, S_IRWXU) != 0) {
                            std::stringstream ss;
                            ss << "mkdir: " << strerror(errno);
                            throw std::logic_error(ss.str());
                        }
                    }
                    *p = '/';
                }
            }
            mkdir(tmp, S_IRWXU);
        }

        bool exists(const std::string &p_directory)
        {
            struct stat buffer;
            return (stat(p_directory.c_str(), &buffer) == 0);
        }

        void copyFile(const std::string &p_source, const std::string &p_destination)
        {
            std::ifstream src(p_source, std::ios::binary);
            std::ofstream dst(p_destination, std::ios::binary);

            dst << src.rdbuf();

            dst.close();
            src.close();
        }

        void remove(const std::string &p_file)
        {
            std::remove(p_file.c_str());
        }
    }

}
