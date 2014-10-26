# provides:
#
# EASYLOGGING++_INCLUDE_DIR
# EASYLOGGING++_FOUND

find_path(EASYLOGGING++_INCLUDE_DIR easylogging++.h
          HINTS "${EASYLOGGING++_ROOT}"
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Easylogging++  DEFAULT_MSG
                                  EASYLOGGING++_INCLUDE_DIR)
