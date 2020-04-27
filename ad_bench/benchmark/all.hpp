#pragma once

#include <utility>

#include <benchmark/benchmark.h>
namespace centered_grad_case {
  namespace direct {
    double compute_cpp(benchmark::State& state);
    double compute_fortran(benchmark::State& state);
  }
}

namespace roe_flux_case {
  namespace direct {
    double compute_cpp(benchmark::State& state);
    double compute_fortran(benchmark::State& state);
  }
}

std::pair<double,double> benchmark_centered_gradient_cpp(int n_iter, int n_cell);
std::pair<double,double> benchmark_centered_gradient_fortran(int n_iter, int n_cell);
std::pair<double,double> benchmark_centered_gradient_codipack_tangent(int n_iter, int n_cell);
std::pair<double,double> benchmark_centered_gradient_tapenade_tangent(int n_iter, int n_cell);
std::pair<double,double> benchmark_centered_gradient_codipack_reverse(int n_iter, int n_cell);
std::pair<double,double> benchmark_centered_gradient_tapenade_reverse(int n_iter, int n_cell);

std::pair<double,double> benchmark_roe_flux_cpp(int n_iter, int n_cell);
std::pair<double,double> benchmark_roe_flux_fortran(int n_iter, int n_cell);
std::pair<double,double> benchmark_roe_flux_codipack_tangent(int n_iter, int n_cell);
std::pair<double,double> benchmark_roe_flux_tapenade_tangent(int n_iter, int n_cell);
std::pair<double,double> benchmark_roe_flux_codipack_reverse(int n_iter, int n_cell);
std::pair<double,double> benchmark_roe_flux_tapenade_reverse(int n_iter, int n_cell);
