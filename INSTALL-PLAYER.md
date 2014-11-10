# Install Player

## Dependencies

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
sudo apt-get install autotools-dev build-essential cmake git subversion libboost-thread-dev libboost-signals-dev libcv2.4 libcv-dev libhighgui2.4 libhighgui-dev libgdk-pixbuf2.0-0 libgdk-pixbuf2.0-dev libgnomecanvas2-0 libgnomecanvas2-dev libgsl0ldbl libgsl0-dev libtool libxmu-dev swig libgtk2.0-dev libplayerc++3.0 libplayerc++3.0-dev libdb5.1-stl libdb5.1-stl-dev
```

## Compile

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

If you are using a 64bit linux, the Player libraries will be installed to ```/usr/local/lib64```
instead of ```/usr/local/lib``` and Player will fail to load the libraries. Add the following lines
to your ```~/.bashrc```:

```
# Player
export PLAYERPATH="/usr/local/lib":"/usr/local/lib64"
```

## CMake

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

## Code Lite

For code completion add the following directory to your tag search path.

```
/usr/local/include/player-3.1
```
