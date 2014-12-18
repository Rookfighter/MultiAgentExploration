# once done this will define
# UNITTEST++_FOUND
# UNITTEST++_INCLUDE_DIRS
# UNITTEST++_LIBRARIES

find_path(UNITTEST++_INCLUDE_DIRS NAMES UnitTest++.h
	HINTS
	"${UNITTEST++_ROOT}/src"
	"${UNITTEST++_ROOT}/include"
	"${UNITTEST++_ROOT}/UnitTest++")
find_library(UNITTEST++_LIBRARIES NAMES UnitTest++ libUnitTest++
	HINTS
	"${UNITTEST++_ROOT}/build"
	"${UNITTEST++_ROOT}/lib"
	"${UNITTEST++_ROOT}/bin")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(UnitTest++ DEFAULT_MSG
                                  UNITTEST++_LIBRARIES UNITTEST++_INCLUDE_DIRS)