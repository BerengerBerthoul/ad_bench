#include<vector>
#include <numeric>
#include <chrono>

#include "tapenade_reverse.hpp"

//using namespace functions;

int main(int argc, char** argv)
{
  // Parameter capture from command
  int n_iter = atoi(argv[1]);
  int n_cell = atoi(argv[2]);
  int n_test = atoi(argv[3]);
 
  std::cout << "Number of iterations: " << n_iter << std::endl;
  std::cout << "Number of cells: " << n_cell << std::endl;

  if (n_test == 0)
  {
    /** 

     1D - CENTERED GRADIENT CASE (REVERSE ONLY)

  **/

    const auto res = benchmark_centered_gradient_tapenade_reverse(n_cell, n_iter); 

    // Result output to console
    std::cout << "=========== 1D centered gradient benchmark ===========\n";
    std::cout << "\tReverse computations\n";
    std::cout << "\t\tEnzyme\t value:"<< res.first << ";\telapsed time (sec): " << res.second << "\n";
  }
  else if (n_test == 1)
  {
    /**

      3D - ROE FLUX CASE (REVERSE ONLY)

    **/

    const auto res = benchmark_roe_flux_tapenade_reverse(n_cell, n_iter); 
  
    // Result output to console
    std::cout << "=========== 3D Roe Flux Benchmark ===========\n";
    std::cout << "\tReverse computations\n";
    std::cout << "\t\tEnzyme\t value:"<< res.first << ";\telapsed time (sec): " << res.second << "\n"; 
  }
  else
  {
    std::cout << "Invalid input for test case in 3rd argument, please choose either 0 or 1" <<std::endl;
  }
}
