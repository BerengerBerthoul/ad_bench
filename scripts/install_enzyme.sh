module purge

module load cmake/3.12.3

# source env and load modules
source /scratchm/rhea/dist/spack/share/spack/setup-env.sh
module load llvm-11.0.1-gcc-10.2.0-csk5bab
module load gcc/8.3

export CC=clang
export CXX=clang++
export FC=gfortran

cd ../ # we are supposed to be in the ROOT_DIR/scripts folder
ROOT_DIR=$(pwd)
ENZYME_DIR=$ROOT_DIR/build/_deps/enzyme # build adol-c in the same place CMake builds the other dependencies
ENZYME_INSTALL_DIR=$ENZYME_DIR/install
ENZYME_SOURCES=$ROOT_DIR/build/_deps/enzyme/Enzyme/enzyme
mkdir -p $ENZYME_DIR
mkdir -p $ENZYME_INSTALL_DIR

cd $ENZYME_DIR
git clone https://github.com/wsmoses/Enzyme
cd Enzyme/enzyme && \
mkdir build && cd build \

cmake -S $ENZYME_SOURCES -DCMAKE_INSTALL_PREFIX:PATH=$ENZYME_INSTALL_DIR -DLLVMDIR=/scratchm/rhea/opt/linux-centos7-broadwell/gcc-10.2.0/llvm-11.0.1-qwyuzvxax4yb4obxbbnu3c6odomdvk34/lib/cmake/llvm

make -v -j48
make install
#export CXXFLAGS="-O3 -march=native -std=c++14 -fopenmp"
#export CFLAGS="-O3 -march=native -fopenmp"
#autoreconf -fi && \
#./configure --prefix=$ADOLC_INSTALL_DIR --with-cxxflags="-O3 -march=native -std=c++14 -fopenmp" && \
#make -v -j24 && \
#make check && \
#make install
