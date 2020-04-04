module load gcc/9.2

#export MESHSIZE=1048576
#export NITER=200
export MESHSIZE=134217728
export NITER=1

rm -v output/zz_mem*
rm -v output/oo_*

time ./xx_directFortranT1.x   $NITER $MESHSIZE > output/oo_directFortranT1
time ./xx_directFortranT2.x   $NITER $MESHSIZE > output/oo_directFortranT2     
time ./xx_directT1.x          $NITER $MESHSIZE > output/oo_directT1           
time ./xx_directT2.x          $NITER $MESHSIZE > output/oo_directT2        
time ./xx_TangentTapenadeT1.x $NITER $MESHSIZE > output/oo_TangentTapenadeT1
time ./xx_TangentTapenadeT2.x $NITER $MESHSIZE > output/oo_TangentTapenadeT2
time ./xx_TangentCoDiPackT1.x $NITER $MESHSIZE > output/oo_TangentCoDiPackT1
time ./xx_TangentCoDiPackT2.x $NITER $MESHSIZE > output/oo_TangentCoDiPackT2
time ./xx_ReverseTapenadeT1.x $NITER $MESHSIZE > output/oo_ReverseTapenadeT1
time ./xx_ReverseTapenadeT2.x $NITER $MESHSIZE > output/oo_ReverseTapenadeT2
time ./xx_ReverseCoDiPackT1.x $NITER $MESHSIZE > output/oo_ReverseCoDiPackT1
time ./xx_ReverseCoDiPackT2.x $NITER $MESHSIZE > output/oo_ReverseCoDiPackT2


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
#mv zz_mem* output/
