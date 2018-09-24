
#include <vector>
#include <iostream>
#include <sstream>
#include <chrono>
#include <ctime>

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
  std::vector<double>  w(im+2*gh);
  std::vector<double> dw(im+2*gh);
  
  /* Initialize a value for w */
  for(int i=0; i < w.size(); i++){w[i] = 1.*i;}
  
  /* 
   * Begin 
   */
  start = std::chrono::system_clock::now();
  for(int ite=0; ite < niter; ite++)
  {
    stencilO2(w, dw, gh);
  }
  end = std::chrono::system_clock::now();
  

  /*
   * End 
   */  
  std::chrono::duration<double> elapsed = (end-start);
  std::cout << " ### Elapsed Normal Direct : " << elapsed.count() << std::endl;
  
}