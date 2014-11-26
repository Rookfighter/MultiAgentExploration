#include "io/ImportYaml.hpp"

#include <easylogging++.h>
#include "io/YamlNode.hpp"
#include "utils/Convert.hpp"

namespace mae
{

    ImportYaml::ImportYaml()
    {
    }

    ImportYaml::~ImportYaml()
    {
    }

    void ImportYaml::resolveImports(const std::string& p_file)
    {
        std::stringstream ss;
        YAML::Emitter emitter(ss);

        YAML::Node importRoot, importNode;

        LOG(INFO) << "loading config file '" << p_file << "'";
        root_ = YAML::LoadFile(p_file);
        importNode = root_[YamlNode::import];

        emitter << root_;

        if(importNode.IsDefined()) {
            if(!importNode.IsSequence())
                throw std::logic_error("import is no sequence");

            for(YAML::Node childNode : importNode) {
                std::string importFile = childNode.as<std::string>();
                LOG(INFO) << "-- importing file '" << importFile << "'";
                importRoot = YAML::LoadFile(importFile);
                emitter << importRoot;
            }
        }
        
        std::string outContent = ss.str();
        replaceSubstring(outContent, "---", "");

        root_ = YAML::Load(outContent);
    }

    YAML::Node& ImportYaml::getRoot()
    {
        return root_;
    }

}
