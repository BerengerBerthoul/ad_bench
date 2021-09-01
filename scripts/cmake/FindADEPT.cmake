# ADEPT_INCLUDE_DIRS - where to find adept_arrays.h, etc.
# COMMON_LIBRARIES - List of fully qualified libraries to link against when using ADEPT.
# ADEPT_FOUND       - Do not attempt to use ADEPT if "no" or undefined.
cmake_minimum_required(VERSION 3.12)

find_path(ADEPT_INCLUDE_DIR NAMES adept_arrays.h)
find_library(COMMON_LIBRARY NAMES libadept.so)
message(ADEPT_INCLUDE_DIR)
message(${ADEPT_INCLUDE_DIR})
message(COMMON_LIBRARY)
message(${COMMON_LIBRARY})

mark_as_advanced(ADEPT_INCLUDE_DIR COMMON_LIBRARY)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(ADEPT
  REQUIRED_VARS
    ADEPT_INCLUDE_DIR COMMON_LIBRARY
)


if (ADEPT_FOUND)
  set(ADEPT_INCLUDE_DIRS "${ADEPT_INCLUDE_DIR}")
  set(COMMON_LIBRARIES "${COMMON_LIBRARY}")

  add_library(ADEPT::ADEPT SHARED IMPORTED)
  set_target_properties(ADEPT::ADEPT PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${ADEPT_INCLUDE_DIR}"
    IMPORTED_LOCATION "${COMMON_LIBRARY}"
  )
endif ()
