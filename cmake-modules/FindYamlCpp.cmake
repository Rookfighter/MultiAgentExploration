# provides:
#
# YAMLCPP_INCLUDE_DIR
# YAMLCPP_LIBRARY
# YAMLCPP_FOUND

find_path(YAMLCPP_INCLUDE_DIR yaml-cpp/yaml.h
          HINTS "${YAMLCPP_ROOT}/include"
)

find_library(YAMLCPP_LIBRARY NAMES yaml-cpp
             HINTS
             "${YAMLCPP_ROOT}/bin"
             "${YAMLCPP_ROOT}/lib"
             "${YAMLCPP_ROOT}/build")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(YamlCpp  DEFAULT_MSG
                                  YAMLCPP_LIBRARY
                                  YAMLCPP_INCLUDE_DIR)
