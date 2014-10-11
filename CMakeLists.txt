cmake_minimum_required(VERSION 2.8)

project(MultiAgentExploration)

set(CMAKE_MODULE_PATH "/usr/local/share/cmake/Modules")
set(CMAKE_CXX_FLAGS "-std=c++11")
add_definitions( -DBOOST_SIGNALS_NO_DEPRECATION_WARNING )
include_directories("src" "lib")
 
# These files include macros that make building against Player easier. They provide
# the PLAYER_ADD* macros
include(UsePlayerC++)
include(UsePlayerPlugin)

# get shared source files
file(GLOB MULTI_AGENT_EXPLORATION_SRC
    "src/algorithm/*.cpp"
    "src/common/*.cpp"
    "src/control/*.cpp")

# add nodecounting client
file(GLOB MAE_TEST_SRC "src/test/*.cpp" "src/test/*.hpp")
PLAYER_ADD_PLAYERCPP_CLIENT(
    Test
    SOURCES ${MULTI_AGENT_EXPLORATION_SRC} ${MAE_TEST_SRC})
