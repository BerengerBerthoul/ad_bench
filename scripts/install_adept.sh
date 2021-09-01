module load cmake/3.12.3
module load gcc/9.2

export CC=gcc
export CXX=g++
export FC=gfortran

cd ../ # we are supposed to be in the ROOT_DIR/scripts folder
ROOT_DIR=$(pwd)
ADEPT_DIR=$ROOT_DIR/build/_deps/adept # build adol-c in the same place CMake builds the other dependencies
ADEPT_INSTALL_DIR=$ADEPT_DIR/install

mkdir -p $ADEPT_DIR
mkdir -p $ADEPT_INSTALL_DIR

cd $ADEPT_DIR
git clone https://github.com/rjhogan/Adept-2.git && \
#git clone https://github.com/coin-or/ADOL-C.git && \
cd Adept-2 && \
#git checkout releases/2.7.2 && \

#export CXXFLAGS="-O1"
export CXXFALGS="-O3 -march=native -DADEPT_STACK_THREAD_UNSAFE"
#export CXXFLAGS="-Ofast -fno-math-errno -march=native -DNDEBUG -fopenmp -fPIC -ffast-math -fno-tree-loop-vectorize"
#export CFLAGS=$CXXFLAGS
export CFLAGS="-O3 -march=native"
autoreconf -fi && \
./configure  --prefix=$ADEPT_INSTALL_DIR CPPFLAGS=$CXXFLAGS && \
make -v -j24 && \
make check && \
make install
