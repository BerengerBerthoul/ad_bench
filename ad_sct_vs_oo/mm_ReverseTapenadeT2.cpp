
#include <vector>
#include <iostream>
#include <sstream>
#include <chrono>
#include <ctime>
#include <cmath>

#include "CfdFortranExternFunction.hpp"

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
  std::vector<double>  rho(im+2*gh);
  std::vector<double>  rhod(im+2*gh);
  std::vector<double>  velx(im+2*gh);
  std::vector<double>  velxd(im+2*gh);
  std::vector<double>  vely(im+2*gh);
  std::vector<double>  velyd(im+2*gh);
  std::vector<double>  velz(im+2*gh);
  std::vector<double>  velzd(im+2*gh);
  std::vector<double>  temp(im+2*gh);
  std::vector<double>  tempd(im+2*gh);
  std::vector<double>  flux1(im+2*gh);
  std::vector<double>  flux1d(im+2*gh);
  std::vector<double>  flux2(im+2*gh);
  std::vector<double>  flux2d(im+2*gh);
  std::vector<double>  flux3(im+2*gh);
  std::vector<double>  flux3d(im+2*gh);
  std::vector<double>  flux4(im+2*gh);
  std::vector<double>  flux4d(im+2*gh);
  std::vector<double>  flux5(im+2*gh);
  std::vector<double>  flux5d(im+2*gh);
  std::vector<double>  surfx(im+2*gh);
  std::vector<double>  surfy(im+2*gh);
  std::vector<double>  surfz(im+2*gh);
  
  /* Initialize a value for all array */
  for(int i=0; i < rho.size(); i++)
  { 
    rho  [i]  = 1.*i; 
    velx [i]  = 1.*i; 
    vely [i]  = 1.*i; 
    velz [i]  = 1.*i; 
    temp [i]  = 1.*i; 
    rhod[i]   = 0.;
    velxd[i]  = 0.;
    velyd[i]  = 0.;
    velzd[i]  = 0.;
    tempd[i]  = 0.;
    surfx[i]  = 1.*i; 
    surfy[i]  = 1.*i; 
    surfz[i]  = 1.*i; 
    flux1d[i] = 1.*i;
    flux2d[i] = 1.*i;
    flux3d[i] = 1.*i;
    flux4d[i] = 1.*i;
    flux5d[i] = 1.*i;
  }
  
  /* 
   * Begin 
   */
  start = std::chrono::system_clock::now();
  for(int ite=0; ite < niter; ite++)
  {
    
    /* Reset array */
    for(int i=0; i < rho.size(); i++)
    {
      flux1d[i] = 1.*i;
      flux2d[i] = 1.*i;
      flux3d[i] = 1.*i;
      flux4d[i] = 1.*i;
      flux5d[i] = 1.*i;
    }
    
    fluxroe_b_(rho.data(), 
               rhod.data(),
               velx.data(), 
               velxd.data(), 
               vely.data(), 
               velyd.data(), 
               velz.data(), 
               velzd.data(), 
               temp.data(), 
               tempd.data(), 
               surfx.data(), 
               surfy.data(), 
               surfz.data(), 
               flux1.data(), 
               flux1d.data(), 
               flux2.data(), 
               flux2d.data(), 
               flux3.data(), 
               flux3d.data(), 
               flux4.data(), 
               flux4d.data(), 
               flux5.data(), 
               flux5d.data(), 
               im, gh );
  }
  end = std::chrono::system_clock::now();
  
  /*
   * End 
   */  
  std::chrono::duration<double> elapsed = (end-start);
  std::cout << " ### Elapsed Tapenade Reverse : " << elapsed.count() << std::endl;
  
  std::cout << " ### Results Tapenade Reverse : " << rhod[im/2] << std::endl;
  
}
