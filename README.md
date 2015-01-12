# Multi Agent Exploration

## Installation

__Attention:__ This project was only tested under __Linux Mint 17__.

### Dependencies

The following dependencies are required to run and compile the project on __Linux__:

* Stage
* yaml-cpp 0.5.1 (in ```lib/```)
* easylogging++ 9.75 (in ```lib/```)
* gcc 4.8 or higher

### Stage

For Stage installation the following dependencies are required:

* FLTK >=1.1
* git
* cmake
* pkg-config
* libpng
* jpeglib
* OpenGLU 

```
sudo apt-get install build-essential git cmake pkg-config libglu1-mesa-dev libpng12-dev libjpeg8-dev libfltk1.1-dev libltdl-dev
```

Now compile and install Stage (used commit ```f9c59ca77f203049979dcf32223e5d2b26c03bf2```):

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
# Stage
export PATH=$PATH:"/usr/local/lib64"
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:"/usr/local/lib":"/usr/local/lib64"
export STAGEPATH="/usr/local/lib":"/usr/local/lib64"
```

Test your installation:

```
# Start stage
cd $STAGE_INSTALL_DIR/worlds
stage simple.world

# Another Test
stage fasr.world

# Test libstageplugin
# (only with Player)
player simple.cfg
```

### CMake

To compile an executable with __libstage__ copy the file ```stage.pc``` from the
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

### Eclipse

For Code Completion add the following directory in **Project Properties > C/C++ General > Paths and Symbols**:

```
/usr/local/include/Stage-4.1
```

### Matplotlib

```
sudo apt-get install python-tk python-matplotlib
```
