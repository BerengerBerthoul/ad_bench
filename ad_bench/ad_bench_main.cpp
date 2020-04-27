#include <vector>
#include <iostream>
#include <chrono>

#include "benchmark/all.hpp"
#include <benchmark/benchmark.h>

template<class F>
void register_benchmark(const char* name, F f, int mesh_size, int nb_meshes) {
  benchmark::RegisterBenchmark(name, f)
    ->RangeMultiplier(2)
    ->Range(mesh_size,mesh_size<<nb_meshes)
    ->Unit(benchmark::kMicrosecond);
  //return f(mesh_size);
}
#define REGISTER_BENCHMARK(f,sz,nb) register_benchmark(#f,f,sz,nb)

int main(int argc, char* argv[]) {
  constexpr int centered_grad_base_size = 64*64*64;
  constexpr int roe_flux_base_size = 64*64*16;
  constexpr int nb_meshes = 0;

  //REGISTER_BENCHMARK(centered_grad_case::direct::compute_fortran, centered_grad_base_size, nb_meshes);
  //REGISTER_BENCHMARK(centered_grad_case::direct::compute_cpp    , centered_grad_base_size, nb_meshes);
  //REGISTER_BENCHMARK(roe_flux_case::direct::compute_fortran     , roe_flux_base_size     , nb_meshes);
  //REGISTER_BENCHMARK(roe_flux_case::direct::compute_cpp         , roe_flux_base_size     , nb_meshes);

  REGISTER_BENCHMARK(centered_grad_case::tangent::compute_tapenade, centered_grad_base_size, nb_meshes);
  REGISTER_BENCHMARK(centered_grad_case::tangent::compute_codipack, centered_grad_base_size, nb_meshes);
  REGISTER_BENCHMARK(roe_flux_case::tangent::compute_tapenade     , roe_flux_base_size     , nb_meshes);
  REGISTER_BENCHMARK(roe_flux_case::tangent::compute_codipack     , roe_flux_base_size     , nb_meshes);

  benchmark::Initialize(&argc, argv);
  benchmark::RunSpecifiedBenchmarks();
  return 0;
}

//void benchmark_centered_gradient(int n_iter, int n_cell, int type) {
//  std::pair<double,double> res;
//
//  std::cout << "=========== 1D centered gradient benchmark ===========\n";
//
//  if (type == 0) {
//    std::cout << "\tDirect computations\n";
//    res = benchmark_centered_gradient_fortran(n_iter,n_cell);
//    std::cout << "\t\tFortran \t value:"<< res.first << ";\telapsed time (sec): " << res.second << "\n";
//
//    res = benchmark_centered_gradient_cpp(n_iter,n_cell);
//    std::cout << "\t\tC++     \t value:"<< res.first << ";\telapsed time (sec): " << res.second << "\n";
//  }
//
//  if (type == 1) {
//    std::cout << "\tTangent computations\n";
//    res = benchmark_centered_gradient_tapenade_tangent(n_iter,n_cell);
//    std::cout << "\t\tTapenade\t value:"<< res.first << ";\telapsed time (sec): " << res.second << "\n";
//
//    res = benchmark_centered_gradient_codipack_tangent(n_iter,n_cell);
//    std::cout << "\t\tCodipack\t value:"<< res.first << ";\telapsed time (sec): " << res.second << "\n";
//  }
//
//  if (type == 2) {
//    std::cout << "\tReverse computations\n";
//    res = benchmark_centered_gradient_tapenade_reverse(n_iter,n_cell);
//    std::cout << "\t\tTapenade\t value:"<< res.first << ";\telapsed time (sec): " << res.second << "\n";
//
//    res = benchmark_centered_gradient_codipack_reverse(n_iter,n_cell);
//    std::cout << "\t\tCodipack\t value:"<< res.first << ";\telapsed time (sec): " << res.second << "\n";
//  }
//}
//
//void benchmark_roe_flux(int n_iter, int n_cell, int type) {
//  std::pair<double,double> res;
//
//  std::cout << "======== Navier-Stokes 3D Roe flux benchmark =========\n";
//
//  if (type == 0) {
//    std::cout << "\tDirect computations\n";
//    res = benchmark_roe_flux_fortran(n_iter,n_cell);
//    std::cout << "\t\tFortran \t value:"<< res.first << ";\telapsed time (sec): " << res.second << "\n";
//
//    res = benchmark_roe_flux_cpp(n_iter,n_cell);
//    std::cout << "\t\tC++     \t value:"<< res.first << ";\telapsed time (sec): " << res.second << "\n";
//  }
//
//  if (type == 1) {
//    std::cout << "\tTangent computations\n";
//    res = benchmark_roe_flux_tapenade_tangent(n_iter,n_cell);
//    std::cout << "\t\tTapenade\t value:"<< res.first << ";\telapsed time (sec): " << res.second << "\n";
//
//    res = benchmark_roe_flux_codipack_tangent(n_iter,n_cell);
//    std::cout << "\t\tCodipack\t value:"<< res.first << ";\telapsed time (sec): " << res.second << "\n";
//  }
//
//  if (type == 2) {
//    std::cout << "\tReverse computations\n";
//    res = benchmark_roe_flux_tapenade_reverse(n_iter,n_cell);
//    std::cout << "\t\tTapenade\t value:"<< res.first << ";\telapsed time (sec): " << res.second << "\n";
//
//    res = benchmark_roe_flux_codipack_reverse(n_iter,n_cell);
//    std::cout << "\t\tCodipack\t value:"<< res.first << ";\telapsed time (sec): " << res.second << "\n";
//  }
//}
//
//int main(int argc, char* argv[]) {
//  if (argc!=5) {
//    std::cout << "ad bench usage: ./ad_bench n_iter n_cell benchmark test(1: centered grad, 2: roe flux) type (0: direct, 1: tangent, 2: reverse) \n";
//    return -1;
//  }
//
//  int n_iter = std::atoi(argv[1]);
//  int n_cell = std::atoi(argv[2]);
//  int bench = std::atoi(argv[3]);
//  int type = std::atoi(argv[4]);
//  std::cout << "Number of iterations: " << n_iter << "\n";
//  std::cout << "Number of cells     : " << n_cell << "\n";
//
//  if (bench ==1) {
//    benchmark_centered_gradient(n_iter,n_cell,type);
//  } else {
//    benchmark_roe_flux(n_iter,n_cell,type);
//  }
//}
