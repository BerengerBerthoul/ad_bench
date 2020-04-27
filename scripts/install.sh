module load cmake/3.12.3
module load gcc/9.2

export CC=gcc
export CXX=g++
export FC=gfortran

cd ..
cmake -S . -Bbuild/ \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_Fortran_FLAGS_RELEASE="-Ofast -fno-math-errno -march=native -DNDEBUG -fopenmp -fPIC -ffast-math -ftree-vectorize -ftree-loop-vectorize -fvect-cost-model=unlimited -mprefer-vector-width=512 -ftree-loop-optimize -ftree-vectorize" \
  -DCMAKE_CXX_FLAGS="-Wall" \
  -DCMAKE_CXX_FLAGS_RELEASE="-Ofast -fno-math-errno -march=native -DNDEBUG -fopenmp -fPIC" \
  -DCMAKE_VERBOSE_MAKEFILE:BOOL=OFF && \

# NOTE: see CMakeLists.txt for other specific optimization flags

# more potential optim flags
#-fopenmp -fopenmp-simd -fPIC -Wall -march=skylake-avx512 -mtune=intel -ffixed-line-length-none -fno-second-underscore -x f95 -frecord-marker=4 -fdefault-real-8 -fdefault-double-8 -funroll-all-loops -fstrict-aliasing  -O4 -fmath-errno  -mfma -ffast-math  -ftree-vectorize -ftree-loop-vectorize -fvect-cost-model=unlimited -mprefer-vector-width=512 -ftree-loop-optimize  -ftree-vectorize -fopt-info -fopt-info-all" # Fortran elsA

#(cd build && make clean)
cmake --build build/ -- -j24 &&

(cd build
./ad_bench)


#valgrind --tool=massif --massif-out-file=zz_mem_directFortranT1   ./xx_directFortranT1.x   $NITER $MESHSIZE  
#valgrind --tool=massif --massif-out-file=zz_mem_directFortranT2   ./xx_directFortranT2.x   $NITER $MESHSIZE  
#valgrind --tool=massif --massif-out-file=zz_mem_directT1         ./xx_directT1.x         $NITER $MESHSIZE  
#valgrind --tool=massif --massif-out-file=zz_mem_directT2         ./xx_directT2.x         $NITER $MESHSIZE
#
#valgrind --tool=massif --massif-out-file=zz_mem_TapenadeTangentT1 ./xx_TangentTapenadeT1.x $NITER $MESHSIZE
#valgrind --tool=massif --massif-out-file=zz_mem_TapenadeTangentT2 ./xx_TangentTapenadeT2.x $NITER $MESHSIZE
#valgrind --tool=massif --massif-out-file=zz_mem_CoDiPackTangentT1 ./xx_TangentCoDiPackT1.x $NITER $MESHSIZE
#valgrind --tool=massif --massif-out-file=zz_mem_CoDiPackTangentT2 ./xx_TangentCoDiPackT2.x $NITER $MESHSIZE
#
#valgrind --tool=massif --massif-out-file=zz_mem_TapenadeReverseT1 ./xx_ReverseTapenadeT1.x $NITER $MESHSIZE
#valgrind --tool=massif --massif-out-file=zz_mem_TapenadeReverseT2 ./xx_ReverseTapenadeT2.x $NITER $MESHSIZE
#valgrind --tool=massif --massif-out-file=zz_mem_CoDiPackReverseT1 ./xx_ReverseCoDiPackT1.x $NITER $MESHSIZE
#valgrind --tool=massif --massif-out-file=zz_mem_CoDiPackReverseT2 ./xx_ReverseCoDiPackT2.x $NITER $MESHSIZE
#
