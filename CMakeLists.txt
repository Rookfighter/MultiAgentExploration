cmake_minimum_required(VERSION 2.8.3)

project(MultiAgentExploration)

set(CMAKE_MODULE_PATH "/usr/local/share/cmake/Modules" "cmake-modules")
set(CMAKE_CXX_FLAGS "-std=c++11")
add_definitions( -DBOOST_SIGNALS_NO_DEPRECATION_WARNING )
include_directories("src")
 
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

file(GLOB CONFIG_FILES "config/*")
file(COPY ${CONFIG_FILES} DESTINATION ${CMAKE_BINARY_DIR})

#foreach( CONFIG_FILE ${CONFIG_FILES} )

#endforeach( CONFIG_FILE ${CONFIG_FILES} )
#add_custom_command(TARGET Test POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy_if_different
#    "${CMAKE_CURRENT_LIST_DIR}/config/easylog.conf"
#    $<TARGET_FILE_DIR:Test>)
