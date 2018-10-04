
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
  std::vector<codi::RealReverse>  rho(im+2*gh);
  std::vector<codi::RealReverse>  velx(im+2*gh);
  std::vector<codi::RealReverse>  vely(im+2*gh);
  std::vector<codi::RealReverse>  velz(im+2*gh);
  std::vector<codi::RealReverse>  temp(im+2*gh);
  std::vector<codi::RealReverse>  flux1(im+2*gh);
  std::vector<codi::RealReverse>  flux2(im+2*gh);
  std::vector<codi::RealReverse>  flux3(im+2*gh);
  std::vector<codi::RealReverse>  flux4(im+2*gh);
  std::vector<codi::RealReverse>  flux5(im+2*gh);
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
  }
  
  
  codi::RealReverse::TapeType& tape = codi::RealReverse::getGlobalTape();
  tape.setActive();    
    
  for(int i=0; i < rho.size(); i++)
  {
    tape.registerInput(rho[i]);
    tape.registerInput(velx[i]);
    tape.registerInput(vely[i]);
    tape.registerInput(velz[i]);
    tape.registerInput(temp[i]);
  }
  
  fluxRoe(rho, velx, vely, velz, temp, 
          surfx, surfy, surfz, 
          flux1, 
          flux2, 
          flux3, 
          flux4, 
          flux5, 
          gh);
  
  /* Prepare output */
  for(int i=0; i < rho.size(); i++)
  {
    tape.registerOutput(flux1[i]);
    tape.registerOutput(flux2[i]);
    tape.registerOutput(flux3[i]);
    tape.registerOutput(flux4[i]);
    tape.registerOutput(flux5[i]);
  }
    
  /* SetUp the end */
  tape.setPassive();
  for(int i=0; i < rho.size(); i++)
  {
    flux1[i].setGradient(1.*i);
    flux2[i].setGradient(1.*i);
    flux3[i].setGradient(1.*i);
    flux4[i].setGradient(1.*i);
    flux5[i].setGradient(1.*i);
  }
  
  /* 
   * Begin 
   */
  start = std::chrono::system_clock::now();
  for(int ite=0; ite < niter; ite++)
  {
    
    for(int i=0; i < rho.size(); i++)
    {
      flux1[i].setGradient(1.*i/(ite+1));
      flux2[i].setGradient(1.*i/(ite+1));
      flux3[i].setGradient(1.*i/(ite+1));
      flux4[i].setGradient(1.*i/(ite+1));
      flux5[i].setGradient(1.*i/(ite+1));
    }
    
    tape.evaluate();
    
  }
  end = std::chrono::system_clock::now();
  
  /*
   * End 
   */  
  std::chrono::duration<double> elapsed = (end-start);
  std::cout << " ### Elapsed Normal Direct : " << elapsed.count() << std::endl;
  
  std::cout << " ### Results CoDiPack Tangent : " << rho[im/2].getGradient() << std::endl;
  
  tape.reset();
  
}
