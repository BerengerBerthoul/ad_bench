#!/bin/bash

# clang compiler and llvm optimizer
export CC="clang++"
export F90="gfortran"

# compiler and optimizer flags
export CCFLAGS="-O2 -fno-vectorize -fno-slp-vectorize -fno-unroll-loops"
export F90LAGS="-O2 -DNDEBUG"

# compile fortran code
$F90 -c *.f90 $F90FLAGS

# compile cpp code
$CC -c *.cpp $CCFLAGS

# link object files into binary
$CC *.o $CCFLAGS -o $1
## invert original code in IR for functions flagged with __enzyme_autodiff wrapper in IR code
#$OPT $OPTFLAGS enzyme_substrate.ll -load=$ENZYMELIB $ENZYMEPM -o enzyme_product.ll -S
#
## optimise inverted IR code
##$OPT enzyme_product.ll -O2 -o enzyme_product_opt.ll -S
#
## produce executable from optimize inverted IR
##$CC enzyme_product_opt.ll -o $2
#$CC enzyme_product.ll -O2 -o $2
#
## produce object file
##$CC -c enzyme_product_opt.ll 
