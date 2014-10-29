cmake_minimum_required(VERSION 2.8.3)

project(MultiAgentExploration)

get_filename_component(YAMLCPP_ROOT "lib/yaml-cpp" ABSOLUTE)
get_filename_component(EASYLOGGING++_ROOT "lib/easylogging++" ABSOLUTE)
set(CMAKE_MODULE_PATH "/usr/local/share/cmake/Modules" "${CMAKE_CURRENT_LIST_DIR}/cmake-modules")

#find required packages
find_package(YamlCpp REQUIRED)
find_package(Easylogging++ REQUIRED)

set(CMAKE_CXX_FLAGS "-std=c++11")
add_definitions( -DBOOST_SIGNALS_NO_DEPRECATION_WARNING )
include_directories("src" "${EASYLOGGING++_INCLUDE_DIR}" "${YAMLCPP_INCLUDE_DIR}")
 
if( ${RELEASE} )
    add_definitions( -DNDEBUG )
endif( ${RELEASE} )

# These files include macros that make building against Player easier. They provide
# the PLAYER_ADD* macros
include(UsePlayerC++)
include(UsePlayerPlugin)

# get shared source files
file(GLOB MULTI_AGENT_EXPLORATION_SRC
    "src/algorithm/*.cpp"
    "src/common/*.cpp"
    "src/simulation/*.cpp"
    "src/test/*.cpp")

# add nodecounting client
PLAYER_ADD_PLAYERCPP_CLIENT(
    ${PROJECT_NAME}
    SOURCES ${MULTI_AGENT_EXPLORATION_SRC} ${MAE_TEST_SRC} "src/main.cpp")
target_link_libraries(${PROJECT_NAME} ${YAMLCPP_LIBRARY})

file(GLOB CONFIG_FILES "config/*")
file(COPY ${CONFIG_FILES} DESTINATION ${CMAKE_BINARY_DIR})
