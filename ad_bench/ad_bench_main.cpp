#include <vector>
#include <iostream>
#include <chrono>

#include "benchmark/all.hpp"
#include <benchmark/benchmark.h>

#include <math.h>

template<class F>
void register_benchmark(const char* name, F f, int mesh_size, int nb_meshes) {
  benchmark::RegisterBenchmark(name, f)
    ->RangeMultiplier(2)
    ->Range(mesh_size,mesh_size<<nb_meshes)
    ->Unit(benchmark::kMicrosecond);
}
#define REGISTER_BENCHMARK(f,sz,nb) register_benchmark(#f,f,sz,nb)

int main(int argc, char* argv[]) {
//  constexpr int centered_grad_base_size = 64*64*64;
//  constexpr int roe_flux_base_size = 64*64*16;
  constexpr int nb_meshes = 0;

  int centered_grad_base_size, roe_flux_base_size;
  int imin;
  int imax;

  std::cout << "Enter min and max exponent for the grid size : [2^imin, 2^imax]" << std::endl;

  std::cout << "imin : ";
  std::cin  >> imin;

  std::cout << "imax : ";
  std::cin >> imax;

  for (int i=imin; i<=imax; ++i)
  {

    //centered_grad_base_size = int(pow (2, i) * pow (2, i)  * pow (2, i));
    //roe_flux_base_size = int(pow (2, i)*pow (2, i)*pow (2, i));

    centered_grad_base_size = int(pow (2, i));
    roe_flux_base_size = int(pow (2, i));


    REGISTER_BENCHMARK(centered_grad_case::direct::compute_fortran  , centered_grad_base_size, nb_meshes);
    REGISTER_BENCHMARK(centered_grad_case::direct::compute_cpp      , centered_grad_base_size, nb_meshes);
    REGISTER_BENCHMARK(roe_flux_case::direct::compute_fortran       , roe_flux_base_size     , nb_meshes);
    REGISTER_BENCHMARK(roe_flux_case::direct::compute_cpp           , roe_flux_base_size     , nb_meshes);

    REGISTER_BENCHMARK(centered_grad_case::tangent::compute_tapenade, centered_grad_base_size, nb_meshes);
    REGISTER_BENCHMARK(centered_grad_case::tangent::compute_codipack, centered_grad_base_size, nb_meshes);
    REGISTER_BENCHMARK(centered_grad_case::tangent::compute_adolc   , centered_grad_base_size, nb_meshes);
    REGISTER_BENCHMARK(centered_grad_case::tangent::compute_adept   , centered_grad_base_size, nb_meshes);
    REGISTER_BENCHMARK(roe_flux_case::tangent::compute_tapenade     , roe_flux_base_size     , nb_meshes);
    REGISTER_BENCHMARK(roe_flux_case::tangent::compute_codipack     , roe_flux_base_size     , nb_meshes);
    REGISTER_BENCHMARK(roe_flux_case::tangent::compute_adolc        , roe_flux_base_size     , nb_meshes);
    REGISTER_BENCHMARK(roe_flux_case::tangent::compute_adept        , roe_flux_base_size     , nb_meshes);


    REGISTER_BENCHMARK(centered_grad_case::reverse::compute_tapenade, centered_grad_base_size, nb_meshes);
    REGISTER_BENCHMARK(centered_grad_case::reverse::compute_codipack, centered_grad_base_size, nb_meshes);
    REGISTER_BENCHMARK(centered_grad_case::reverse::compute_adolc   , centered_grad_base_size, nb_meshes);
    REGISTER_BENCHMARK(centered_grad_case::reverse::compute_adept   , centered_grad_base_size, nb_meshes);
    REGISTER_BENCHMARK(roe_flux_case::reverse::compute_tapenade     , roe_flux_base_size     , nb_meshes);
    REGISTER_BENCHMARK(roe_flux_case::reverse::compute_codipack     , roe_flux_base_size     , nb_meshes);
    REGISTER_BENCHMARK(roe_flux_case::reverse::compute_adolc        , roe_flux_base_size     , nb_meshes);
    REGISTER_BENCHMARK(roe_flux_case::reverse::compute_adept        , roe_flux_base_size     , nb_meshes);

  }

  benchmark::Initialize(&argc, argv);
  benchmark::RunSpecifiedBenchmarks();
  
  return 0;
}
