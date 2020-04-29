# ADOLC_INCLUDE_DIRS - where to find adolc.h, etc.
# FSCOMMON_LIBRARIES - List of fully qualified libraries to link against when using ADOLC.
# ADOLC_FOUND       - Do not attempt to use ADOLC if "no" or undefined.
cmake_minimum_required(VERSION 3.12)

find_path(ADOLC_INCLUDE_DIR NAMES adolc/adolc.h)
find_library(FSCOMMON_LIBRARY NAMES libadolc.so)
message(ADOLC_INCLUDE_DIR)
message(${ADOLC_INCLUDE_DIR})

mark_as_advanced(ADOLC_INCLUDE_DIR FSCOMMON_LIBRARY)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(ADOLC
  REQUIRED_VARS
    ADOLC_INCLUDE_DIR FSCOMMON_LIBRARY
)


if (ADOLC_FOUND)
  set(ADOLC_INCLUDE_DIRS "${ADOLC_INCLUDE_DIR}")
  set(FSCOMMON_LIBRARIES "${FSCOMMON_LIBRARY}")

  add_library(ADOLC::ADOLC SHARED IMPORTED)
  set_target_properties(ADOLC::ADOLC PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${ADOLC_INCLUDE_DIR}"
    IMPORTED_LOCATION "${FSCOMMON_LIBRARY}"
  )
endif ()
