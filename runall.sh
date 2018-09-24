
export MESHSIZE=1048576
export NITER=200

rm -v output/zz_mem*
rm -v output/oo_*

time ./xx_directCoDiPackT1.x  $NITER $MESHSIZE > output/oo_directCoDiPackT1   
time ./xx_directCoDiPackT2.x  $NITER $MESHSIZE > output/oo_directCoDiPackT2   
time ./xx_directFortanT1.x    $NITER $MESHSIZE > output/oo_directFortanT1     
time ./xx_directFortanT2.x    $NITER $MESHSIZE > output/oo_directFortanT2     
time ./xx_directT1.x          $NITER $MESHSIZE > output/oo_directT1           
time ./xx_directT2.x          $NITER $MESHSIZE > output/oo_directT2        
time ./xx_TangentCoDiPackT1.x $NITER $MESHSIZE > output/oo_TangentCoDiPackT1
time ./xx_TangentCoDiPackT2.x $NITER $MESHSIZE > output/oo_TangentCoDiPackT2
time ./xx_TangentTapenadeT1.x $NITER $MESHSIZE > output/oo_TangentTapenadeT1
time ./xx_TangentTapenadeT2.x $NITER $MESHSIZE > output/oo_TangentTapenadeT2
time ./xx_ReverseCoDiPackT1.x $NITER $MESHSIZE > output/oo_ReverseCoDiPackT1
time ./xx_ReverseCoDiPackT2.x $NITER $MESHSIZE > output/oo_ReverseCoDiPackT2
time ./xx_ReverseTapenadeT1.x $NITER $MESHSIZE > output/oo_ReverseTapenadeT1
time ./xx_ReverseTapenadeT2.x $NITER $MESHSIZE > output/oo_ReverseTapenadeT2


valgrind --tool=massif --massif-out-file=zz_mem_directCoDiPackT1 ./xx_directCoDiPackT1.x $NITER $MESHSIZE  
valgrind --tool=massif --massif-out-file=zz_mem_directCoDiPackT2 ./xx_directCoDiPackT2.x $NITER $MESHSIZE  
valgrind --tool=massif --massif-out-file=zz_mem_directFortanT1   ./xx_directFortanT1.x   $NITER $MESHSIZE  
valgrind --tool=massif --massif-out-file=zz_mem_directFortanT2   ./xx_directFortanT2.x   $NITER $MESHSIZE  
valgrind --tool=massif --massif-out-file=zz_mem_directT1         ./xx_directT1.x         $NITER $MESHSIZE  
valgrind --tool=massif --massif-out-file=zz_mem_directT2         ./xx_directT2.x         $NITER $MESHSIZE

valgrind --tool=massif --massif-out-file=zz_mem_CoDiPackTangentT1 ./xx_TangentCoDiPackT1.x $NITER $MESHSIZE
valgrind --tool=massif --massif-out-file=zz_mem_CoDiPackTangentT2 ./xx_TangentCoDiPackT2.x $NITER $MESHSIZE

valgrind --tool=massif --massif-out-file=zz_mem_TapenadeTangentT1 ./xx_TangentTapenadeT1.x $NITER $MESHSIZE
valgrind --tool=massif --massif-out-file=zz_mem_TapenadeTangentT2 ./xx_TangentTapenadeT2.x $NITER $MESHSIZE



valgrind --tool=massif --massif-out-file=zz_mem_CoDiPackReverseT1 ./xx_ReverseCoDiPackT1.x $NITER $MESHSIZE
valgrind --tool=massif --massif-out-file=zz_mem_CoDiPackReverseT2 ./xx_ReverseCoDiPackT2.x $NITER $MESHSIZE

valgrind --tool=massif --massif-out-file=zz_mem_TapenadeReverseT1 ./xx_ReverseTapenadeT1.x $NITER $MESHSIZE
valgrind --tool=massif --massif-out-file=zz_mem_TapenadeReverseT2 ./xx_ReverseTapenadeT2.x $NITER $MESHSIZE

mv zz_mem* output/
