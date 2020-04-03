#include <vector>
#include <iostream>
#include <chrono>

#include "benchmark/cpp_direct.hpp"
#include "benchmark/codipack_tangent.hpp"
#include "benchmark/codipack_reverse.hpp"

#include "benchmark/fortran_direct.hpp"
#include "benchmark/tapenade_tangent.hpp"
//#include "benchmark/tapenade_reverse.hpp"

int main(int argc, char* argv[]) {
  if (argc!=3) {
    std::cout << "ad bench usage: ./ad_bench n_iter n_cell\n";
    return -1;
  }

  int n_iter = std::atoi(argv[1]);
  int n_cell = std::atoi(argv[2]);

  double duration;

  std::cout << "number of iterations: " << n_iter << "\n";
  std::cout << "number of cells     : " << n_cell << "\n";


  std::cout << "=========================================\n";
  std::cout << "===== 1D centered gradient benchmark ====\n";
  std::cout << "=========================================\n";

  std::cout << "========== Direct computations ==========\n";
  duration = benchmark_centered_gradient_cpp(n_iter,n_cell);
  std::cout << "C++     elapsed time (ms): " << duration << "\n\n";

  duration = benchmark_centered_gradient_fortran(n_iter,n_cell);
  std::cout << "Fortran elapsed time (ms): " << duration << "\n\n";

  std::cout << "========== Tangent computations ==========\n";
  duration = benchmark_centered_gradient_codipack_tangent(n_iter,n_cell);
  std::cout << "CodiPack elapsed time (ms): " << duration << "\n\n";

  duration = benchmark_centered_gradient_tapenade_tangent(n_iter,n_cell);
  std::cout << "Tapenade elapsed time (ms): " << duration << "\n\n";

  std::cout << "========== Reverse computations ==========\n";
  duration = benchmark_centered_gradient_codipack_reverse(n_iter,n_cell);
  std::cout << "CodiPack elapsed time (ms): " << duration << "\n\n";

  //duration = benchmark_centered_gradient_tapenade_reverse(n_iter,n_cell);
  //std::cout << "Tapenade elapsed time (ms): " << duration << "\n\n";




  std::cout << "=========================================\n";
  std::cout << "== Navier-Stokes 3D Roe flux benchmark ==\n";
  std::cout << "=========================================\n";

  std::cout << "========== Direct computations ==========\n";
  duration = benchmark_roe_flux_cpp(n_iter,n_cell);
  std::cout << "C++     elapsed time (ms): " << duration << "\n\n";

  duration = benchmark_roe_flux_fortran(n_iter,n_cell);
  std::cout << "Fortran elapsed time (ms): " << duration << "\n\n";

  std::cout << "========== Tangent computations ==========\n";
  duration = benchmark_roe_flux_codipack_tangent(n_iter,n_cell);
  std::cout << "CodiPack elapsed time (ms): " << duration << "\n\n";

  duration = benchmark_roe_flux_tapenade_tangent(n_iter,n_cell);
  std::cout << "Tapenade elapsed time (ms): " << duration << "\n\n";

  std::cout << "========== Reverse computations ==========\n";
  duration = benchmark_roe_flux_codipack_reverse(n_iter,n_cell);
  std::cout << "CodiPack elapsed time (ms): " << duration << "\n\n";

  //duration = benchmark_roe_flux_tapenade_reverse(n_iter,n_cell);
  //std::cout << "Tapenade elapsed time (ms): " << duration << "\n\n";
}
