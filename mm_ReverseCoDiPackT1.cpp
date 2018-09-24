
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
  std::vector<codi::RealReverse>  w(im+2*gh); // Meme resultats en Reverse ...
  std::vector<codi::RealReverse> dw(im+2*gh); // Meme resultats en Reverse ...
  
  /* Initialize a value for w */
  for(int i=0; i < w.size(); i++)
  {
    w[i] = 1.*i;
  }
  
  /* 
   * Begin 
   */
  start = std::chrono::system_clock::now();
  for(int ite=0; ite < niter; ite++)
  {
    
    codi::RealReverse::TapeType& tape = codi::RealReverse::getGlobalTape();
    tape.setActive();    
    
    for(int i=0; i < w.size(); i++)
    {
      tape.registerInput(w[i]);
    }
    
    stencilO2(w, dw, gh);
    
    /* Prepare output */
    for(int i=0; i < w.size(); i++)
    {
      tape.registerOutput(dw[i]);
    }
    
    /* SetUp the end */
    tape.setPassive();
    for(int i=0; i < w.size(); i++)
    {
      dw[i].setGradient(1.*i);
    }
    tape.evaluate();
    
    if(ite != niter - 1) tape.reset();
    
  }
  end = std::chrono::system_clock::now();
  
  /*
   * End 
   */  
  std::chrono::duration<double> elapsed = (end-start);
  std::cout << " ### Elapsed CoDiPack Reverse : " << elapsed.count() << std::endl;
  
  std::cout << " ### Results CoDiPack Reverse : " << w[im/2].getGradient() << std::endl;
  
}
