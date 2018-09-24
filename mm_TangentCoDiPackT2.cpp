
#include <vector>
#include <iostream>
#include <sstream>
#include <chrono>
#include <ctime>
#include <cmath>

#include <codi.hpp>
#include "CfdFunction.hpp"

int main(int nargs, char** args)
{
  /* General parse parameter */
  int niter = std::atoi(args[1]);
  int im    = std::atoi(args[2]);
  int gh    = 2;
  
  std::cout << " ####################################################### " << std::endl;
  std::cout << " -> Niter : " << niter << std::endl;
  std::cout << " -> im    : " << im    << std::endl;
  std::cout << " ####################################################### " << std::endl;
  
  /* Timer */
  std::chrono::time_point<std::chrono::system_clock> start, end;
  
  /* Initialisation */
  std::vector<codi::RealForward>  rho(im+2*gh);
  std::vector<codi::RealForward>  velx(im+2*gh);
  std::vector<codi::RealForward>  vely(im+2*gh);
  std::vector<codi::RealForward>  velz(im+2*gh);
  std::vector<codi::RealForward>  temp(im+2*gh);
  std::vector<codi::RealForward>  flux1(im+2*gh);
  std::vector<codi::RealForward>  flux2(im+2*gh);
  std::vector<codi::RealForward>  flux3(im+2*gh);
  std::vector<codi::RealForward>  flux4(im+2*gh);
  std::vector<codi::RealForward>  flux5(im+2*gh);
  std::vector<double>  surfx(im+2*gh);
  std::vector<double>  surfy(im+2*gh);
  std::vector<double>  surfz(im+2*gh);
  
  /* Initialize a value for all array */
  for(int i=0; i < rho.size(); i++)
  { 
    rho  [i] = 1.*i; 
    velx [i] = 1.*i; 
    vely [i] = 1.*i; 
    velz [i] = 1.*i; 
    temp [i] = 1.*i; 
    surfx[i] = 1.*i; 
    surfy[i] = 1.*i; 
    surfz[i] = 1.*i; 
    rho[i] .setGradient(1.*i);
    velx[i].setGradient(1.*i);
    vely[i].setGradient(1.*i);
    velz[i].setGradient(1.*i);
    temp[i].setGradient(1.*i);
  }
  
  /* 
   * Begin 
   */
  start = std::chrono::system_clock::now();
  for(int ite=0; ite < niter; ite++)
  {
    fluxRoe(rho, velx, vely, velz, temp, 
            surfx, surfy, surfz, 
            flux1, 
            flux2, 
            flux3, 
            flux4, 
            flux5, 
            gh);
  }
  end = std::chrono::system_clock::now();
  
  /*
   * End 
   */  
  std::chrono::duration<double> elapsed = (end-start);
  std::cout << " ### Elapsed Normal Direct : " << elapsed.count() << std::endl;
  
  std::cout << " ### Results CoDiPack Tangent : " << flux1[im/2].getGradient() << std::endl;
  
}
