cmake_minimum_required(VERSION 2.8.3)

project(MultiAgentExploration)

get_filename_component(YAMLCPP_ROOT "lib/yaml-cpp" ABSOLUTE)
get_filename_component(EASYLOGGING++_ROOT "lib/easylogging++" ABSOLUTE)
set(CMAKE_MODULE_PATH "/usr/local/share/cmake/Modules" "${CMAKE_CURRENT_LIST_DIR}/cmake-modules")

#find required packages
include(FindPkgConfig)
find_package(YamlCpp REQUIRED)
find_package(Easylogging++ REQUIRED)
pkg_search_module( STAGE REQUIRED stage )

set(CMAKE_CXX_FLAGS "-std=c++11")
add_definitions( -DBOOST_SIGNALS_NO_DEPRECATION_WARNING )
include_directories("src"
                    "${STAGE_INCLUDE_DIRS}"
                    "${EASYLOGGING++_INCLUDE_DIR}"
                    "${YAMLCPP_INCLUDE_DIR}")
link_directories("${STAGE_LIBRARY_DIRS}")
 
if( ${RELEASE} )
    add_definitions( -DNDEBUG )
endif( ${RELEASE} )

# get source files
file(GLOB MULTI_AGENT_EXPLORATION_SRC
    "src/algorithm/*.cpp"
    "src/algorithm-rt/*.cpp"
    "src/algorithm-compass/*.cpp"
    "src/utils/*.cpp"
    "src/simulation/*.cpp"
    "src/app/*.cpp"
    "src/test/*.cpp")

# add executeable
add_executable( ${PROJECT_NAME} ${MULTI_AGENT_EXPLORATION_SRC} "src/main.cpp")
target_link_libraries( ${PROJECT_NAME} ${STAGE_LIBRARIES} ${YAMLCPP_LIBRARY})

file(GLOB CONFIG_FILES "config")
file(GLOB WORLD_FILES "world")
file(COPY ${CONFIG_FILES} DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${WORLD_FILES} DESTINATION ${CMAKE_BINARY_DIR})
