
#include <vector>
#include <iostream>
#include <sstream>
#include <chrono>
#include <ctime>

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
  std::vector<double>  w(im+2*gh);
  std::vector<double>  dw(im+2*gh);
  std::vector<double>  rho(im+2*gh);
  std::vector<double>  velx(im+2*gh);
  std::vector<double>  vely(im+2*gh);
  std::vector<double>  velz(im+2*gh);
  std::vector<double>  temp(im+2*gh);
  std::vector<double>  flux1(im+2*gh);
  std::vector<double>  flux2(im+2*gh);
  std::vector<double>  flux3(im+2*gh);
  std::vector<double>  flux4(im+2*gh);
  std::vector<double>  flux5(im+2*gh);
  std::vector<double>  surfx(im+2*gh);
  std::vector<double>  surfy(im+2*gh);
  std::vector<double>  surfz(im+2*gh);
  
  /* Initialize a value for all array */
  for(int i=0; i < w.size(); i++){ w    [i] = 1.*i; }
  for(int i=0; i < w.size(); i++){ dw   [i] = 1.*i; }
  for(int i=0; i < w.size(); i++){ rho  [i] = 1.*i; }
  for(int i=0; i < w.size(); i++){ velx [i] = 1.*i; }
  for(int i=0; i < w.size(); i++){ vely [i] = 1.*i; }
  for(int i=0; i < w.size(); i++){ velz [i] = 1.*i; }
  for(int i=0; i < w.size(); i++){ temp [i] = 1.*i; }
  for(int i=0; i < w.size(); i++){ surfx[i] = 1.*i; }
  for(int i=0; i < w.size(); i++){ surfy[i] = 1.*i; }
  
  /* 
   * Begin 
   */
  start = std::chrono::system_clock::now();
  for(int ite=0; ite < niter; ite++)
  {
    fluxroe_(rho.data(), 
             velx.data(), 
             vely.data(), 
             velz.data(), 
             temp.data(), 
             surfx.data(), 
             surfy.data(), 
             surfz.data(), 
             flux1.data(), 
             flux2.data(), 
             flux3.data(), 
             flux4.data(), 
             flux5.data(), 
             im, gh );
  }
  end = std::chrono::system_clock::now();
  

  /*
   * End 
   */  
  std::chrono::duration<double> elapsed = (end-start);
  std::cout << " ### Elapsed CoDiPack Direct : " << elapsed.count() << std::endl;
  
}
