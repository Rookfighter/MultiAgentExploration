# Multi Agent Exploration

## Installation

__Attention:__ This project was only tested under __Linux Mint 17__.

### Requirements

The following dependencies are required to run and compile the project on __Linux__:

* Player
* Stage
* yaml-cpp (__lib__ subdirectory)
* easylogging++ (__lib__ subdirectory)
* gcc 4.8 or higher

### Player/Stage

For Player installation the following dependencies are required:

* autotools
* libboost-thread
* libboost-signals
* libcv 2.4
* libhighgui 2.4
* libgdk-pixbuf 2.0
* libgnomecanvas 2
* libgsl0
* libtool
* libxmu
* swig
* libgtk2.0
* libltdl
* libplayerc++ 3.0

```
sudo apt-get install autotools-dev build-essential cmake git subversion libboost-thread-dev libboost-signals-dev libcv2.4 libcv-dev libhighgui2.4 libhighgui-dev libgdk-pixbuf2.0-0 libgdk-pixbuf2.0-dev libgnomecanvas2-0 libgnomecanvas2-dev libgsl0ldbl libgsl0-dev libtool libxmu-dev swig libgtk2.0-dev libltdl7 libltdl-dev libplayerc++3.0 libplayerc++3.0-dev libdb5.1-stl libdb5.1-stl-dev
```

For Stage installation the following dependencies are required:

* FLTK >=1.1
* git
* cmake
* pkg-config
* libpng
* jpeglib
* OpenGLU 

```
sudo apt-get install git cmake pkg-config libglu1-mesa libglu1-mesa-dev libpng12-0 libpng12-dev libjpeg8-dev libfltk1.3 libfltk1.3-dev freeglut3 freeglut3-dev
```

Now compile and install __Player__ (used revision __9140__):

```
svn co svn://svn.code.sf.net/p/playerstage/svn/code/player/trunk player-svn
cd player-svn
svn up -r9140
mkdir build
cd build
cmake ..
make
sudo make install
```

Now compile and install __Stage__ (used commit __f9c59ca77f203049979dcf32223e5d2b26c03bf2__):

```
git clone https://github.com/rtv/Stage.git
cd Stage
git checkout f9c59ca77f203049979dcf32223e5d2b26c03bf2
mkdir build
cd build
cmake ..
make
sudo make install
```

If you are using a 64bit linux, the Stage libraries will be installed to ```/usr/local/lib64```
instead of ```/usr/local/lib``` and Stage will fail to load the libraries. Add the following lines
to your ```~/.bashrc```:

```
# Player/Stage
export PATH=$PATH:"/usr/local/lib64"
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:"/usr/local/lib":"/usr/local/lib64"
export PLAYERPATH="/usr/local/lib":"/usr/local/lib64"
export STAGEPATH="/usr/local/lib":"/usr/local/lib64"
```

Test your installation:

```
#Start stage
cd $STAGE_INSTALL_DIR/worlds
stage simple.world

#Test with stage plugin
stage fasr.world

#Other test:
player simple.cfg
```

### CMake

To compile your player client with cmake add the following lines to your
CMakeLists.txt file:

```
# set search path of cmake
set(CMAKE_MODULE_PATH "/usr/local/share/cmake/Modules")

# These files include macros that make building against Player easier. They provide
# the PLAYER_ADD* macros
include(UsePlayerC++)
include(UsePlayerPlugin)

# add a client which should be be compiled
PLAYER_ADD_PLAYERCPP_CLIENT(${CLIENT_NAME}
    SOURCES ${CLIENT_SRC})
```

Player throws many deprecation warnings because of __Boost.Signals__. To disable
these warnings add the following line to your CMakeLists.txt:

```
add_definitions( -DBOOST_SIGNALS_NO_DEPRECATION_WARNING )
```

To compile an executable with __libstage__ copy the file __stage.pc__ from the
Stage build directory to ```/usr/lib/pkgconfig```. Now add the following lines
to your CMakeLists.txt:

```
include(FindPkgConfig)
pkg_search_module( STAGE REQUIRED stage )

include_directories( ${STAGE_INCLUDE_DIRS} )
link_directories( ${STAGE_LIBRARY_DIRS} )

# create an executable and link libstage to each
add_executable( ${PROJECT_NAME} ${PROJECT_SRC} )
target_link_libraries( ${PROJECT_NAME} ${STAGE_LIBRARIES} )
```

If you are compiling with C++11 (required for this project), then ```stage.hh```
will give you many warnings about literals. In C++11 strings should not be
concatened like this ```"bla"m"bla"```. Instead it should be like this
```"bla" m "bla"```.
To get rid of these warnings open the file ```/usr/local/include/Stage4.1/stage.hh```
and replace ```"m"``` with ```" m "```. All warnings should be gone now.

### Code Lite

For code completion add the following dirextory to your tag search path.

```
/usr/local/include/player-3.1
```
