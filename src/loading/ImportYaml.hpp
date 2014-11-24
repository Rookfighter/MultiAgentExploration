#ifndef MAE_IMPORT_YAML_HPP
#define MAE_IMPORT_YAML_HPP

#include <yaml-cpp/yaml.h>

namespace mae
{

    class ImportYaml
    {
    private:
        YAML::Node root_;
    public:
        ImportYaml();
        ~ImportYaml();

        void resolveImports(const std::string& p_file);
        YAML::Node& getRoot();
    };

}

#endif
