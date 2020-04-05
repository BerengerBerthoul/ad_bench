module load cmake/3.12.3
module load gcc/9.2

export CC=gcc
export CXX=g++
export FC=gfortran

cd ..
cmake -S . -Bbuild/ &&

cmake --build build/ -- -j24 &&


## launch unit tests ##
#134217728=512*512*512
(cd build
./ad_bench 1 134217728 1 0
./ad_bench 1 134217728 1 1
./ad_bench 1 134217728 1 2
./ad_bench 1 134217728 2 0
./ad_bench 1 134217728 2 1
#./ad_bench 1 134217728 2 2

#./ad_bench 200 1048576 1 0
#./ad_bench 200 1048576 1 1
#./ad_bench 200 1048576 1 2
#./ad_bench 200 1048576 2 0
#./ad_bench 200 1048576 2 1
./ad_bench 200 1048576 2 2
)
