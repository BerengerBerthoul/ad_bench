module load cmake/3.12.3
module load gcc/9.2

export CC=gcc
export CXX=g++
export FC=gfortran

cd ../ # we are supposed to be in the ROOT_DIR/scripts folder
ROOT_DIR=$(pwd)
ADOLC_DIR=$ROOT_DIR/build/_deps/adolc # build adol-c in the same place CMake builds the other dependencies
ADOLC_INSTALL_DIR=$ADOLC_DIR/install

mkdir -p $ADOLC_DIR
mkdir -p $ADOLC_INSTALL_DIR

cd $ADOLC_DIR
git clone https://github.com/coin-or/ADOL-C.git && \
cd ADOL-C && \
git checkout releases/2.7.2 && \

export CXXFLAGS="-O3 -march=native -std=c++14 -fopenmp"
export CFLAGS="-O3 -march=native -fopenmp"
autoreconf -fi && \
./configure --prefix=$ADOLC_INSTALL_DIR --with-cxxflags="-O3 -march=native -std=c++14 -fopenmp" && \
make -v -j24 && \
make check && \
make install
