
# Nettoyage 
rm -v *.o

# > On fait les fortran en premier 
ifort -c Fortran/*.f90 -fno-alias -fn-alias  -qopt-report5 -qopt-report-phase=vec,loop 
mv *.o Fortran/

# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#################################################################################
# > Direct Normal type = Double 
icpc -c mm_DirectT1.cpp -I../CoDiPack-master/include -std=c++14 -w 
icpc mm_DirectT1.o  -o xx_directT1.x
#################################################################################

#################################################################################
# > Direct - FORTRAN FOR THE FUCKING WIN
icpc -c mm_DirectFortranT1.cpp -I../CoDiPack-master/include -std=c++14 -w 
icpc mm_DirectFortranT1.o Fortran/*.o -o xx_directFortanT1.x
#################################################################################

#################################################################################
# > Direct Normal type = Double 
icpc -c mm_DirectT2.cpp -I../CoDiPack-master/include -std=c++14 -w 
icpc mm_DirectT2.o  -o xx_directT2.x
#################################################################################

#################################################################################
# > Direct - FORTRAN FOR THE FUCKING WIN
icpc -c mm_DirectFortranT2.cpp -I../CoDiPack-master/include -std=c++14 -w 
icpc mm_DirectFortranT2.o Fortran/*.o -o xx_directFortanT2.x
#################################################################################
# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#################################################################################
# > Tangent Tapenade type 
icpc -c mm_TangentTapenadeT1.cpp -I../CoDiPack-master/include -std=c++14 -w 
icpc mm_TangentTapenadeT1.o Fortran/*.o -o xx_TangentTapenadeT1.x
#################################################################################

#################################################################################
# > Tangent CoDiPack type 
icpc -c mm_TangentCoDiPackT1.cpp -I../CoDiPack-master/include -std=c++14 -w 
icpc mm_TangentCoDiPackT1.o  -o xx_TangentCoDiPackT1.x
#################################################################################

#################################################################################
# > Tangent Tapenade type 
icpc -c mm_TangentTapenadeT2.cpp -I../CoDiPack-master/include -std=c++14 -w 
icpc mm_TangentTapenadeT2.o Fortran/*.o -o xx_TangentTapenadeT2.x
#################################################################################

#################################################################################
# > Tangent CoDiPack type 
icpc -c mm_TangentCoDiPackT2.cpp -I../CoDiPack-master/include -std=c++14 -w 
icpc mm_TangentCoDiPackT2.o  -o xx_TangentCoDiPackT2.x
#################################################################################
# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#################################################################################
# > Reverse CoDiPack type 
icpc -c mm_ReverseCoDiPackT1.cpp -I../CoDiPack-master/include -std=c++14 -w 
icpc mm_ReverseCoDiPackT1.o  -o xx_ReverseCoDiPackT1.x
#################################################################################

#################################################################################
# > Reverse CoDiPack type 
icpc -c mm_ReverseCoDiPackT2.cpp -I../CoDiPack-master/include -std=c++14 -w 
icpc mm_ReverseCoDiPackT2.o  -o xx_ReverseCoDiPackT2.x
#################################################################################

#################################################################################
# > Reverse Tapenade type 
icpc -c mm_ReverseTapenadeT1.cpp -I../CoDiPack-master/include -std=c++14 -w 
icpc mm_ReverseTapenadeT1.o Fortran/*.o -o xx_ReverseTapenadeT1.x
#################################################################################

#################################################################################
# > Reverse Tapenade type 
icpc -c mm_ReverseTapenadeT2.cpp -I../CoDiPack-master/include -std=c++14 -w 
icpc mm_ReverseTapenadeT2.o Fortran/*.o -o xx_ReverseTapenadeT2.x
#################################################################################
# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
