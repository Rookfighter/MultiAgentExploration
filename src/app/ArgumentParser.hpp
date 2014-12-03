#ifndef MAE_ARGUMENT_PARSER_HPP
#define MAE_ARGUMENT_PARSER_HPP

#include <string>

namespace mae
{

    class ArgumentParser
    {
    private:
        static std::string configFile_;
        static int terminationMinutes_;
        static double terminationCoverage_;
        static bool useGui_;

        ArgumentParser() { }
        ~ArgumentParser() { }

        static void reset();
        static void printHelp();
    public:

        static bool parse(int argc, char** argv);

        static const std::string& getConfigFile();
        static int getTerminationMinutes();
        static double getTerminationCoverage();
        static bool useGUI();
    };

}

#endif
