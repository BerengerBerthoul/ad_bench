## Global config ##
cmake_minimum_required(VERSION 3.12)
set(CMAKE_FORTRAN_COMPILER_ENV_VAR gfortran)
set(CMAKE_CPP_COMPILER clang++)
## Project ##
project(
  ad_bench
  DESCRIPTION   "Algorithmic differentiation benchmark between source code transformation and operator overloading"
  VERSION 0.1.0
  LANGUAGES     CXX Fortran
)

## Dependencies ##
include(FetchContent)
#include(LLVMIRUtil)
find_package(Git REQUIRED)
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

include_directories(${googlebench_SOURCE_DIR}/include)
set(INC_PATH ${googlebench_SOURCE_DIR}/include)

message(INC_PATH)
message(${INC_PATH})

### Enzyme ###
set(CMAKE_MODULE_PATH "scripts/cmake" ${CMAKE_MODULE_PATH})
find_package(ENZYME REQUIRED)
#message(ENZYME_INCLUDE_DIR)
#message(${ENZYME_INCLUDE_DIR})


### BOOST library ###
#find_package(Boost REQUIRED system)
#target_link_libraries( ${Boost_SYSTEM_LIBRARY})
#set ( BOOST libboost_system.so )
#FIND_PACKAGE( Boost 1.58 COMPONENTS program_options REQUIRED )
#INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )
#message(Boost_INCLUDE_DIR)
#message(${Boost_INCLUDE_DIR})

## header file directory
#set(adept_include_dir /home/jorge/Documents/programmation/Adept/install/include)
## link directories
#set ( PROJECT_LINK_LIBS_2  libadept.so )
#link_directories(/home/jorge/Documents/programmation/Adept/install/lib/)

## Find files ##
set(src_dir ${PROJECT_SOURCE_DIR}/${PROJECT_NAME})
set(include_dir ${PROJECT_SOURCE_DIR})

message(PROJECT_SOURCE_DIR)
message(${PROJECT_SOURCE_DIR})

file(GLOB_RECURSE cpp_and_fortran_files
  "${src_dir}/routines/*.cpp"
  "${src_dir}/routines/*.f90"
)

#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "/scratchm/jnunezra/AD/gbench/enzyme/sources/build")


message(CMAKE_MODULE_PATH)
message(${CMAKE_MODULE_PATH})

#add_library(MyObj OBJECT ${cpp_and_fortran_files})


###### Alternative 1 : using custom commands for the whole process

message("Trying out the custom commands")

add_custom_command(OUTPUT "roe_flux_b.o" "centered_gradient_b.o"
                  COMMAND clang++ -c ${PROJECT_SOURCE_DIR}/ad_bench/routines/tapenade/*.f90
                  COMMENT "Compiling Tapenade Fortran routines")

add_custom_command(OUTPUT "tapenade_reverse.o"
                  COMMAND clang++ -c ${PROJECT_SOURCE_DIR}/ad_bench/routines/tapenade/tapenade_reverse.cpp -isystem ${INC_PATH}
                  COMMENT "Compiling Tapenade C++ routines"
                  )

add_custom_command(OUTPUT  "input.ll"
                   #DEPENDS "main_gbench.cpp"
                   COMMAND clang++ ${PROJECT_SOURCE_DIR}/ad_bench/main_gbench.cpp -S -isystem ${INC_PATH} -emit-llvm -o input.ll
                   COMMENT "Emitting llvm IR for AD with Enzyme")

add_custom_command(OUTPUT  "product.ll"
                   DEPENDS "input.ll"
                   COMMAND opt -enable-new-pm=0 input.ll -load=${PROJECT_SOURCE_DIR}/build/_deps/enzyme/install/lib/ClangEnzyme-11.so -enzyme -o product.ll -S 
                   #COMMAND opt -enable-new-pm=0 input.ll -load=ENZYME::ENZYME -enzyme -o product.ll -S 
                   COMMENT "Applying Enzyme to emitted llvm IR code")

add_custom_command(OUTPUT  "product.o"
                   DEPENDS "product.ll"
                   COMMAND clang++ -c product.ll  
                   COMMENT "Compiling differentiated code")

add_custom_command(OUTPUT ${PROJECT_NAME}
                  DEPENDS product.o
                  #COMMAND clang++ $<TARGET_OBJECTS:MyObj> product.o -isystem ${INC_PATH} -L/scratchm/jnunezra/AD/gbench/enzyme/gbench/install/lib64 -lbenchmark -lpthread -o ${PROJECT_NAME}
                  COMMAND clang++ *.o -isystem ${INC_PATH} -L${PROJECT_SOURCE_DIR}/build/_deps/googlebench-build/src -lbenchmark -lpthread -o ${PROJECT_NAME}
                  COMMENT "Linking all object files and creating executable"
                  )

add_custom_target(test_target ALL
                  DEPENDS
                  roe_flux_b.o centered_gradient_b.o tapenade_reverse.o input.ll product.ll product.o ${PROJECT_NAME})

########## Alternative 2 using the traditional cmake commands plus some custom commands for applying enzyme
########## Currently fails (Enzymelib.so not linking) 

#add_executable(${PROJECT_NAME} $<TARGET_OBJECTS:MyObj> product.o)                                               
#SET_SOURCE_FILES_PROPERTIES(
#       product.o
#       PROPERTIES
#       EXTERNAL_OBJECT true
#       GENERATED true
#       )
# 
#     SET_TARGET_PROPERTIES(
#       ${PROJECT_NAME}
#       PROPERTIES
#       LINKER_LANGUAGE CXX 
#       )
#
#target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_14)
#target_include_directories(${PROJECT_NAME} PUBLIC
#  $<BUILD_INTERFACE:${include_dir}>
#  $<INSTALL_INTERFACE:include/${PROJECT_NAME}>
#  ${INC_PATH}
#)
#
#message($<BUILD_INTERFACE:${include_dir})
#message($<INSTALL_INTERFACE:include/${PROJECT_NAME}>)
#
#target_link_libraries(${PROJECT_NAME}
#  PUBLIC
#  benchmark
#  gtest
#  ENZYME::ENZYME
#)

