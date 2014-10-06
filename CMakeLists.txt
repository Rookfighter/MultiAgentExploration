cmake_minimum_required(VERSION 2.8)

project(MultiAgentExploration)

set(CMAKE_MODULE_PATH "/usr/local/share/cmake/Modules")
set(CMAKE_CXX_FLAGS "-std=c++11")
add_definitions( -DBOOST_SIGNALS_NO_DEPRECATION_WARNING )
include_directories("src/share")
 
# These files include macros that make building against Player easier. They provide
# the PLAYER_ADD* macros
include(UsePlayerC++)
include(UsePlayerPlugin)

# get shared source files
file(GLOB MAE_SHARE_SRC
    "src/share/algorithm/*.cpp"
    "src/share/common/*.cpp"
    "src/share/robots/*.cpp")

# add nodecounting client
file(GLOB MAE_NODE_COUNTING_SRC "src/nodecounting/*.cpp" "src/nodecounting/*.hpp")
PLAYER_ADD_PLAYERCPP_CLIENT(
    Nodecounting
    SOURCES ${MAE_SHARE_SRC} ${MAE_NODE_COUNTING_SRC})
