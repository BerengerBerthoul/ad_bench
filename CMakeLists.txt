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
set(codipack_include_dir ${PROJECT_SOURCE_DIR}/codipack)

file(GLOB_RECURSE cpp_and_fortran_files
  "${src_dir}/*.cpp"
  "${src_dir}/*.f90"
)

## Compiler flags ##
# TODO move
add_compile_options(-Wall -fmax-errors=2 -Ofast -fno-math-errno -march=native -DNDEBUG -fopenmp -fPIC)


## Executable ##
add_executable(${PROJECT_NAME} ${cpp_and_fortran_files})
#target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_17)
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
