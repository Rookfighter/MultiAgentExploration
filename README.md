# Exploration with Ant Bots

## Installation

### Required Programs

The following programs are required to run and compile the project on __Windows__:

* webots
* MSYS

### Code Lite

To compile the project in Code Lite the following environment variables must be set in the project settings:

```
WEBOTS_HOME=WebotsRootDir
PATH=MSYSBinDir;WebotsMinGWBinDir;$PATH
SHELL=sh.exe
``` 

For code completion add the following directories to the project search path:

```
WebotsRootDir\include\controller\cpp
WebotsRootDir\mingw\lib\gcc\i686-w64-mingw32\4.8.0\include\c++
```

### Player / Stage

For Player/Stage installation on Linux Mint 17 install the following dependencies:

```
sudo apt-get install build-essential cmake git subversion python python-dev
sudo apt-get install libcv2.4 libcv-dev libhighgui2.4 libhighgui-dev libgdk-pixbuf2.0-0 libgdk-pixbuf2.0-dev libgnomecanvas2-0 libgnomecanvas2-dev libgsl0ldbl libgsl0-dev
sudo apt-get install libtool libxmu-dev swig freeglut3 freeglut3-dev libgtk2.0-dev
sudo apt-get install libjpeg-dev libpng-dev libglu1-mesa-dev libfltk1.1 libfltk1.1-dev libltdl7 libltdl-dev libplayerc++ libplayerc++3.0-dev freeglut3 freeglut3-dev
```

Now compile and install Player:

```
svn co svn://svn.code.sf.net/p/playerstage/svn/code/player/trunk player
cd player
mkdir build
cd build
cmake ..
make
sudo make install
```

Now compile and install Stage:

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
instead of ```/usr/local/lib``` and Stage will fail to load the libraries. Add the following
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
