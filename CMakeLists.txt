## Global config ##
cmake_minimum_required(VERSION 3.12)
set(CMAKE_FORTRAN_COMPILER_ENV_VAR gfortran)

## Project ##
project(
  ad_bench
  DESCRIPTION   "Algorithmic differentiation benchmark between source code transformation and operator overloading"
  VERSION 0.1.0
  LANGUAGES     CXX Fortran
)

## Dependencies ##
### CoDiPack ###
include(FetchContent)
find_package(Git REQUIRED)
FetchContent_Declare(
  codipack
  GIT_REPOSITORY https://github.com/SciCompKL/CoDiPack.git
  GIT_TAG        v1.9.1
)
FetchContent_GetProperties(codipack)
if(NOT codipack_POPULATED)
  FetchContent_Populate(codipack)
endif()
set(codipack_include_dir ${codipack_SOURCE_DIR}/include)
### Google Test (needed by Google Benchmark) ###
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.10.0
)
FetchContent_GetProperties(googletest)
if(NOT googletest_POPULATED)
  FetchContent_Populate(googletest)
  add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
endif()
### Google Benchmark ###
FetchContent_Declare(
  googlebench
  GIT_REPOSITORY https://github.com/google/benchmark.git
  GIT_TAG        v1.5.0
)
FetchContent_GetProperties(googlebench)
if(NOT googlebench_POPULATED)
  FetchContent_Populate(googlebench)
  add_subdirectory(${googlebench_SOURCE_DIR} ${googlebench_BINARY_DIR})
endif()
### ADOLC ###
#set(CMAKE_MODULE_PATH "scripts/cmake" ${CMAKE_MODULE_PATH})
#find_package(ADOLC REQUIRED)
#message(ADOLC_INCLUDE_DIR)
#message(${ADOLC_INCLUDE_DIR})
set(CMAKE_MODULE_PATH "scripts/cmake" ${CMAKE_MODULE_PATH})
find_package(ADOLC REQUIRED)
message(ADOLC_INCLUDE_DIR)
message(${ADOLC_INCLUDE_DIR})


### BOOST library ###
#find_package(Boost REQUIRED system)
#target_link_libraries( ${Boost_SYSTEM_LIBRARY})
set ( BOOST libboost_system.so )
#FIND_PACKAGE( Boost 1.58 COMPONENTS program_options REQUIRED )
#INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )
#message(Boost_INCLUDE_DIR)
#message(${Boost_INCLUDE_DIR})

## header file directory
#set(adolc_include_dir /home/jorge/Documents/programmation/ADOL-C/install_fast3/include)
## link directories
#set ( PROJECT_LINK_LIBS libboost_system.so libadolc.so )
#link_directories(/home/jorge/Documents/programmation/ADOL-C/install_fast3/lib64)

### Adept ###
set(CMAKE_MODULE_PATH "scripts/cmake" ${CMAKE_MODULE_PATH})
find_package(ADEPT REQUIRED)
message(ADEPT_INCLUDE_DIR)
message(${ADEPT_INCLUDE_DIR})



## header file directory
#set(adept_include_dir /home/jorge/Documents/programmation/Adept/install/include)
## link directories
#set ( PROJECT_LINK_LIBS_2  libadept.so )
#link_directories(/home/jorge/Documents/programmation/Adept/install/lib/)





## Find files ##
set(src_dir ${PROJECT_SOURCE_DIR}/${PROJECT_NAME})
set(include_dir ${PROJECT_SOURCE_DIR})

file(GLOB_RECURSE cpp_and_fortran_files
  "${src_dir}/*.cpp"
  "${src_dir}/*.f90"
)

## Executable ##
add_executable(${PROJECT_NAME} ${cpp_and_fortran_files})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_14)
target_include_directories(${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${codipack_include_dir}>
#  $<BUILD_INTERFACE:${adolc_include_dir}>
#  $<BUILD_INTERFACE:${adept_include_dir}>
  $<BUILD_INTERFACE:${include_dir}>
  $<INSTALL_INTERFACE:include/${PROJECT_NAME}>
)
target_link_libraries(${PROJECT_NAME}
  PUBLIC
    benchmark
    gtest
#    ${PROJECT_LINK_LIBS}
#    ${PROJECT_LINK_LIBS_2}
#    "-Wgtel,--rpatgtgth -Wtgergl,/home/jorge/Dogrethercuments/programmation/ADOL-C/install2/lib64 -L/home/gtjorge/Documents/programmation/ADOL-C/install2/lib64 -ladolc"
    ${BOOST}
#    ${Boost_LIBRARIES}
    ADOLC::ADOLC
    ADEPT::ADEPT
)

### Install ###
install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}Targets
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES DESTINATION include
)
#install(EXPORT ${PROJECT_NAME}Targets
#  FILE ${PROJECT_NAME}Config.cmake # no dependency
#  NAMESPACE ${PROJECT_NAME}::
#  DESTINATION lib/cmake/${PROJECT_NAME}
#)
install(DIRECTORY ${src_dir} DESTINATION include FILES_MATCHING PATTERN "*.hpp")
