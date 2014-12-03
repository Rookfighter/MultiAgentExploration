#include <iostream>
#include <sstream>
#include <stdexcept>
#include <unistd.h>
#include "app/ArgumentParser.hpp"

namespace mae
{
    std::string ArgumentParser::configFile_ = "";
    int ArgumentParser::terminationMinutes_ = -1;
    double ArgumentParser::terminationCoverage_ = -1;
    bool ArgumentParser::useGui_ = true;


    void ArgumentParser::reset()
    {
        configFile_ = "";
        terminationMinutes_ = -1;
        terminationCoverage_ = -1;
        useGui_ = true;
    }

    bool ArgumentParser::parse(int argc, char** argv)
    {

        char c;
        bool gaveFile = false;
        while((c = getopt(argc, argv, "hgf:t:c:")) != -1) {
            switch(c) {
            case 'h':
                printHelp();
                return false;
            case 'g':
                useGui_ = false;
                break;
            case 'f':
                gaveFile = true;
                configFile_ = optarg;
                break;
            case 't':
                terminationMinutes_ = std::stoi(optarg);
                break;
            case 'c':
                terminationCoverage_ = ((double) std::stoi(optarg)) / 100.0;
                break;
            default:
                std::stringstream ss;
                ss << "unknown argument '-" << (char) c << "'";
                throw std::logic_error(ss.str());
            }
        }

        if(!gaveFile)
            throw std::logic_error("no config file given");

        return true;
    }

    void ArgumentParser::printHelp()
    {
        std::cout << "usage: MultiAgentExploration -f <filename> [...]" << std::endl;
        std::cout << "arguments:" << std::endl;
        std::cout << "  -h                  help" << std::endl;
        std::cout << "  -g                  disable GUI" << std::endl;
        std::cout << "  -f <filename>       config file (required)" << std::endl;
        std::cout << "  -c <percent>        coverage termination condition" << std::endl;
        std::cout << "  -t <minutes>        time termination condition" << std::endl;
    }

    const std::string& ArgumentParser::getConfigFile()
    {
        return configFile_;
    }

    int ArgumentParser::getTerminationMinutes()
    {
        return terminationMinutes_;
    }

    double ArgumentParser::getTerminationCoverage()
    {
        return terminationCoverage_;
    }

    bool ArgumentParser::useGUI()
    {
        return useGui_;
    }

}
