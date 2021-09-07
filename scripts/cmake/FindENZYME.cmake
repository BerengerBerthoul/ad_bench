# FSCOMMON_LIBRARIES - List of fully qualified libraries to link against when using Enzyme.
# ENZYME_FOUND       - Do not attempt to use ENZYME if "no" or undefined.
cmake_minimum_required(VERSION 3.12)

#find_path(ADOLC_INCLUDE_DIR NAMES adolc/adolc.h)
find_library(FSCOMMON_LIBRARY NAMES ClangEnzyme-11.so)
#message(ADOLC_INCLUDE_DIR)
#message(${ADOLC_INCLUDE_DIR})

mark_as_advanced(FSCOMMON_LIBRARY)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(ENZYME
  REQUIRED_VARS
    FSCOMMON_LIBRARY
)


if (ENZYME_FOUND)
  message(enzyme found!!!!!!)
  #set(ADOLC_INCLUDE_DIRS "${ADOLC_INCLUDE_DIR}")
  set(FSCOMMON_LIBRARIES "${FSCOMMON_LIBRARY}")

  add_library(ENZYME::ENZYME SHARED IMPORTED)
  message("ENZYMEENZYME")
  message(${FSCOMMON_LIBRARY})
  set_target_properties(ENZYME::ENZYME PROPERTIES
    #INTERFACE_INCLUDE_DIRECTORIES "${ADOLC_INCLUDE_DIR}"
    IMPORTED_LOCATION "${FSCOMMON_LIBRARY}"
  )
endif ()
