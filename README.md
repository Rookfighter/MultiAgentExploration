# Multi Agent Exploration

## Installation

__Attention:__ This project was only tested under __Linux Mint 17__. However, it should also work on __Ubuntu 14.04__.

The following dependencies are required to run and compile the project:

* Stage
* yaml-cpp 0.5.1 (in ```lib/```)
* easylogging++ 9.75 (in ```lib/```)
* gcc 4.8 or higher

For compiling the 3 libraries the following components have to be installed via APT:

```
sudo apt-get install build-essential git subversion cmake pkg-config libglu1-mesa-dev libpng12-dev libjpeg8-dev libfltk1.1-dev libltdl-dev libboost-dev libboost-thread-dev
```

The dependencies yaml-cpp and easylogging++ can be installed using the ```get-deps``` shell script in ```scripts/```. If the script does not work, you have to install those libraries manually.

### yaml-cpp

Yaml-pp is expected to be in the ```lib/yaml-cpp/``` subdirectory of the repository. You can find yaml-cpp [here](https://code.google.com/p/yaml-cpp/). Execute the following commands to install yaml-cpp:

```
cd <path-to-repo>
mkdir -p lib/yaml-cpp
cd lib/
wget "https://yaml-cpp.googlecode.com/files/yaml-cpp-0.5.1.tar.gz" -O "yaml-cpp.tar.gz"
tar xzf yaml-cpp.tar.gz -C yaml-cpp
rm -f yaml-cpp.tar.gz
cd yaml-cpp
mkdir -p bin
cd bin
cmake ..
make
```

The compiled libraries should be in ```lib/yaml-cpp/bin```. Alternatively you can use ```build/``` or ```lib/``` as build directories.

### Easylogging++

Easylogging++ is a header-only logging framework and is expected to be in the ```lib/easylogging++/``` subdirectory of the repository. You can find it [here](http://easylogging.muflihun.com/). Execute the following commands to install Easylogging++:

```
cd <path-to-repo>
mkdir -p lib/easylogging++
cd lib/
wget "https://github.com/easylogging/easyloggingpp/releases/download/v9.75/easyloggingpp_v9.75.tar.gz" -O "easylogging++.tar.gz"
tar xzf easylogging++.tar.gz -C easylogging++
rm -f easylogging++.tar.gz
```

The header file ```easylogging++.h``` should be in ```lib/easylogging++/```.

### Stage

Unlike the other libraries Stage will be installed globally. You can find the Stage repository [here](https://github.com/rtv/Stage). For Stage installation the following dependencies are required:

* FLTK >=1.1
* git
* cmake
* pkg-config
* libpng
* jpeglib
* OpenGLU 

Compile and install Stage (used commit ```f9c59ca77```):

```
cd <any-dir>
git clone https://github.com/rtv/Stage.git
cd Stage
git checkout f9c59ca77f203049979dcf32223e5d2b26c03bf2
mkdir build
cd build
cmake ..
make
sudo make install
```

If you are using a 64bit linux, the Stage libraries will be installed to ```/usr/local/lib64``` and Stage will fail to load the libraries during runtime. Also the file ```stage.pc``` will be installed to ```/usr/local/lib64/pkgconfig``` and pkgconfig won't be able to find the Stage libs for linking. Add the following lines
to your ```~/.bashrc``` to solve these problems:

```
# Stage
export PATH=$PATH:"/usr/local/lib64"
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:"/usr/local/lib":"/usr/local/lib64"
export STAGEPATH="/usr/local/lib":"/usr/local/lib64"
export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:"/usr/local/lib64/pkgconfig"
```

To verify your installation, you can try to run the following scenarios. ```<stage-install-dir>``` is in most cases ```/usr/local/share/stage/```.

```
# Start stage
cd <stage-install-dir>/worlds
stage simple.world

# Another Test
stage fasr.world
```

Because this project is compiled with C++11 , then ```stage.hh```
will give you many warnings about literals. In C++11 strings should not be
concatened like this ```"text1"strVar"text2"```. Instead it should be like this
```"text1" strVar "text2"```.

To get rid of these warnings open the file ```/usr/local/include/Stage4.1/stage.hh```
and replace ```"m"``` with ```" m "```. All warnings should be gone now.

### Compile Project

After installing all the dependencies, compilation should be no problem. Checkout the source code and execute the following commands:

```
cd <path-to-repo>
mkdir build
cd build
cmake ..
make
```

The executeable ```MultiAgentExploration``` should now be available in ```build/```. CMake also copies the directiories ```config/``` and ```ẁorld/``` into the build directory. Those are required for executing the binary.

### Python scripts

For executing the python scripts in the subdirectory ```scripts/``` Python 2.7 and Matplotlib are required. The following dependencies have to be installed via APT:

```
sudo apt-get install python2.7 python-tk python-matplotlib
```

## Execution

The binary of the project can only be executed with the working directory in the build directory, e.g. ```build/```, because of relative paths to config files. Specify with ```-f <configfile>``` a config file, which describes the scenario that should be simulated. Those config files can be found in subdirectories of the ```config/``` directory (copied by cmake). ```-h``` prints an help text for additional commands.

Example of execution:

```
cd <build-dir>
./MultiAgentExploration -f config/office/nodecounting-4.yaml
```

The config files have the follwing pattern ```config/$TERRAIN/$ALGORITHM-$ROBOTCOUNT.yaml```.

## Additional Information

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

### Headers

Stage headers can be found in ```/usr/local/include/Stage-4.1```.
