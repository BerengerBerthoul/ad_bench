## Global config ##
cmake_minimum_required(VERSION 3.12)
set(CMAKE_FORTRAN_COMPILER_ENV_VAR gfortran)

## Project ##
project(
  ad_bench VERSION 0.1.0
  DESCRIPTION	"Algorithmic differentiation benchmark between source code transformation and operator overloading"
  LANGUAGES	CXX Fortran
)

## Find files ##
set(src_dir ${PROJECT_SOURCE_DIR}/${PROJECT_NAME})
set(include_dir ${PROJECT_SOURCE_DIR})
set(codipack_include_dir ${PROJECT_SOURCE_DIR}/external/codipack)

file(GLOB_RECURSE cpp_and_fortran_files
  "${src_dir}/*.cpp"
  "${src_dir}/*.f90"
)

## Compiler flags ##
# TODO move
#add_compile_options(-Wall -fmax-errors=2 -Ofast -fno-math-errno -march=native -DNDEBUG -fopenmp -fPIC)
#add_compile_options(-Wall -fmax-errors=2 -ftemplate-backtrace-limit=0 -Winline -O2 -march=native -DNDEBUG -finline-limit=100000 --param large-function-insns=10000 --param large-stack-frame-growth=10000 --param inline-unit-growth=10000 --param early-inlining-insns=1000 --param max-early-inliner-iterations=1000 -fopenmp -fPIC)
#add_compile_options(-Wall -fmax-errors=2 -ftemplate-backtrace-limit=0 -Winline -O2 -march=native -DNDEBUG -finline-limit=100000 --param large-function-insns=10000 --param inline-unit-growth=200 --param inline-min-speedup=1 -fopenmp -fPIC)

set(CMAKE_CXX_FLAGS  "-std=c++14 -Wall -fmax-errors=2")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Ofast -fno-math-errno -march=native -DNDEBUG -finline-limit=100000 --param large-function-insns=10000 --param large-stack-frame-growth=10000 --param inline-unit-growth=10000 --param early-inlining-insns=1000 --param max-early-inliner-iterations=1000 -fopenmp -fPIC")
set(CMAKE_Fortran_FLAGS_RELEASE "${CMAKE_Fortran_FLAGS_RELEASE} -Ofast -fno-math-errno -march=native -DNDEBUG -fopenmp -fPIC")
set(CMAKE_BUILD_TYPE "Release")


## Executable ##
add_executable(${PROJECT_NAME} ${cpp_and_fortran_files})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_14)
target_include_directories(${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${codipack_include_dir}>
  $<BUILD_INTERFACE:${include_dir}>
  $<INSTALL_INTERFACE:include/${PROJECT_NAME}>
)

### Install ###
install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}Targets
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES DESTINATION include
)
install(EXPORT ${PROJECT_NAME}Targets
  FILE ${PROJECT_NAME}Config.cmake # no dependency
  NAMESPACE ${PROJECT_NAME}::
  DESTINATION lib/cmake/${PROJECT_NAME}
)
install(DIRECTORY ${src_dir} DESTINATION include FILES_MATCHING PATTERN "*.hpp")
