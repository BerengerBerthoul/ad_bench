#include <vector>
#include <numeric>
#include <chrono>

#include "routines/enzyme/enzyme_benchmarks.hpp"
#include "routines/tapenade/tapenade_reverse.hpp"

#include <benchmark/benchmark.h>
#include <math.h>

template<class F>
void register_benchmark(const char* name, F f, int mesh_size, int nb_meshes) 
{
  benchmark::RegisterBenchmark(name, f)
    ->RangeMultiplier(2)
    ->Range(mesh_size,mesh_size<<nb_meshes)
    ->Unit(benchmark::kMicrosecond);
}
#define REGISTER_BENCHMARK(f,sz,nb) register_benchmark(#f,f,sz,nb)

int main(int argc, char** argv)
{
  // Parameter capture from command line
  int imin, imax, n_cell;

  //std::cout << "Number of cells: " << n_cell << std::endl;

  constexpr int nb_meshes = 0;

  std::cout << "Enter min and max exponent for the grid size : [2^imin,2^imax]" << std::endl;

  std::cout << "imin : ";
  std::cin  >> imin;

  std::cout << "imax : ";
  std::cin >> imax;

  for (int i=imin; i<=imax; ++i)
  {

    n_cell= int(pow (2, i));
    // Initialize and run benchmarks
    REGISTER_BENCHMARK(tapenade_centered_gradient, n_cell, nb_meshes);
    REGISTER_BENCHMARK(enzyme_centered_gradient, n_cell, nb_meshes);
    REGISTER_BENCHMARK(tapenade_roe_flux, n_cell, nb_meshes);
    REGISTER_BENCHMARK(enzyme_roe_flux, n_cell, nb_meshes);

  }
  benchmark::Initialize(&argc, argv);
  benchmark::RunSpecifiedBenchmarks();
  return 0;
}
