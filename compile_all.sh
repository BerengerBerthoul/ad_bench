
# Nettoyage 
rm -v *.o

# > On fait les fortran en premier 
ifort -c Fortran/*.f90  -fpp -w -w90 -w95 -fno-alias -unroll0 -qopt-report5 -qopt-report-phase=vec,loop -override-limits -fno-alias -fno-fnalias -qopt-report5 -qopt-report-phase=vec,loop,cg -xhost -fma -qopenmp-simd -qopt-report-embed

mv *.o Fortran/

# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#################################################################################
# > Direct Normal type = Double 
icpc -c mm_DirectT1.cpp -I../CoDiPack-master/include -std=c++14 -Wall -fmax-errors=1 -qoverride-limits -qopt-report5 -qopt-report-phase=vec,loop -xCORE-AVX2 -fma -qopenmp -align -inline-forceinline -O3 -fp-model fast=2
icpc mm_DirectT1.o -liomp5 -o xx_directT1.x
#################################################################################

#################################################################################
# > Direct - FORTRAN FOR THE FUCKING WIN
icpc -c mm_DirectFortranT1.cpp -I../CoDiPack-master/include -std=c++14 -Wall -fmax-errors=1 -qoverride-limits -qopt-report5 -qopt-report-phase=vec,loop -xCORE-AVX2 -fma -qopenmp -align -inline-forceinline -O3 -fp-model fast=2
icpc mm_DirectFortranT1.o Fortran/*.o -liomp5 -o xx_directFortanT1.x
#################################################################################

#################################################################################
# > Direct Normal type = Double 
icpc -c mm_DirectT2.cpp -I../CoDiPack-master/include -std=c++14 -Wall -fmax-errors=1 -qoverride-limits -qopt-report5 -qopt-report-phase=vec,loop -xCORE-AVX2 -fma -qopenmp -align -inline-forceinline -O3 -fp-model fast=2
icpc mm_DirectT2.o -liomp5 -o xx_directT2.x
#################################################################################

#################################################################################
# > Direct - FORTRAN FOR THE FUCKING WIN
icpc -c mm_DirectFortranT2.cpp -I../CoDiPack-master/include -std=c++14 -Wall -fmax-errors=1 -qoverride-limits -qopt-report5 -qopt-report-phase=vec,loop -xCORE-AVX2 -fma -qopenmp -align -inline-forceinline -O3 -fp-model fast=2
icpc mm_DirectFortranT2.o Fortran/*.o -liomp5 -o xx_directFortanT2.x
#################################################################################
# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#################################################################################
# > Tangent Tapenade type 
icpc -c mm_TangentTapenadeT1.cpp -I../CoDiPack-master/include -std=c++14 -Wall -fmax-errors=1 -qoverride-limits -qopt-report5 -qopt-report-phase=vec,loop -xCORE-AVX2 -fma -qopenmp -align -inline-forceinline -O3 -fp-model fast=2
icpc mm_TangentTapenadeT1.o Fortran/*.o -liomp5 -o xx_TangentTapenadeT1.x
#################################################################################

#################################################################################
# > Tangent CoDiPack type 
icpc -c mm_TangentCoDiPackT1.cpp -I../CoDiPack-master/include -std=c++14 -Wall -fmax-errors=1 -qoverride-limits -qopt-report5 -qopt-report-phase=vec,loop -xCORE-AVX2 -fma -qopenmp -align -inline-forceinline -O3 -fp-model fast=2
icpc mm_TangentCoDiPackT1.o -liomp5 -o xx_TangentCoDiPackT1.x
#################################################################################

#################################################################################
# > Tangent Tapenade type 
icpc -c mm_TangentTapenadeT2.cpp -I../CoDiPack-master/include -std=c++14 -Wall -fmax-errors=1 -qoverride-limits -qopt-report5 -qopt-report-phase=vec,loop -xCORE-AVX2 -fma -qopenmp -align -inline-forceinline -O3 -fp-model fast=2
icpc mm_TangentTapenadeT2.o Fortran/*.o -liomp5 -o xx_TangentTapenadeT2.x
#################################################################################

#################################################################################
# > Tangent CoDiPack type 
icpc -c mm_TangentCoDiPackT2.cpp -I../CoDiPack-master/include -std=c++14 -Wall -fmax-errors=1 -qoverride-limits -qopt-report5 -qopt-report-phase=vec,loop -xCORE-AVX2 -fma -qopenmp -align -inline-forceinline -O3 -fp-model fast=2
icpc mm_TangentCoDiPackT2.o -liomp5 -o xx_TangentCoDiPackT2.x
#################################################################################
# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#################################################################################
# > Reverse CoDiPack type 
icpc -c mm_ReverseCoDiPackT1.cpp -I../CoDiPack-master/include -std=c++14 -Wall -fmax-errors=1 -qoverride-limits -qopt-report5 -qopt-report-phase=vec,loop -xCORE-AVX2 -fma -qopenmp -align -inline-forceinline -O3 -fp-model fast=2
icpc mm_ReverseCoDiPackT1.o -liomp5 -o xx_ReverseCoDiPackT1.x
#################################################################################

#################################################################################
# > Reverse CoDiPack type 
icpc -c mm_ReverseCoDiPackT2.cpp -I../CoDiPack-master/include -std=c++14 -Wall -fmax-errors=1 -qoverride-limits -qopt-report5 -qopt-report-phase=vec,loop -xCORE-AVX2 -fma -qopenmp -align -inline-forceinline -O3 -fp-model fast=2
icpc mm_ReverseCoDiPackT2.o -liomp5 -o xx_ReverseCoDiPackT2.x
#################################################################################

#################################################################################
# > Reverse Tapenade type 
icpc -c mm_ReverseTapenadeT1.cpp -I../CoDiPack-master/include -std=c++14 -Wall -fmax-errors=1 -qoverride-limits -qopt-report5 -qopt-report-phase=vec,loop -xCORE-AVX2 -fma -qopenmp -align -inline-forceinline -O3 -fp-model fast=2
icpc mm_ReverseTapenadeT1.o Fortran/*.o -liomp5 -o xx_ReverseTapenadeT1.x
#################################################################################

#################################################################################
# > Reverse Tapenade type 
icpc -c mm_ReverseTapenadeT2.cpp -I../CoDiPack-master/include -std=c++14 -Wall -fmax-errors=1 -qoverride-limits -qopt-report5 -qopt-report-phase=vec,loop -xCORE-AVX2 -fma -qopenmp -align -inline-forceinline -O3 -fp-model fast=2
icpc mm_ReverseTapenadeT2.o Fortran/*.o -liomp5 -o xx_ReverseTapenadeT2.x
#################################################################################
# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
