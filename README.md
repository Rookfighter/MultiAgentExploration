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

For Player/Stage installation the following dependencies are required:

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
* freeglut3
* libgtk2.0
* libjpeg
* libpng 12
* libglu1-mesa
* libfltk 1.1
* libltdl
* libplayerc++ 3.0

```
sudo apt-get install autotools-dev build-essential cmake git subversion libboost-thread-dev libboost-signals-dev libcv2.4 libcv-dev libhighgui2.4 libhighgui-dev libgdk-pixbuf2.0-0 libgdk-pixbuf2.0-dev libgnomecanvas2-0 libgnomecanvas2-dev libgsl0ldbl libgsl0-dev libjpeg8-dev libtool libxmu-dev swig freeglut3 freeglut3-dev libfltk1.1 libfltk1.1-dev libgtk2.0-dev libltdl7 libltdl-dev libpng12-0 libpng12-dev libplayerc++3.0 libplayerc++3.0-dev libglu1-mesa libglu1-mesa-dev libdb5.1-stl libdb5.1-stl-dev
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

Now compile and install __Stage__:

```
wget -O Stage-4.1.1.tar.gz https://github.com/rtv/Stage/archive/v4.1.1.tar.gz
tar -xzf Stage-4.1.1.tar.gz
cd Stage-4.1.1
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

### Code Lite

For code completion add the following dirextory to your tag search path.

```
/usr/local/include/player-3.1
```
