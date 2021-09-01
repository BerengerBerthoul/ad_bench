#pragma once

#include <utility>

#include <benchmark/benchmark.h>
namespace centered_grad_case {
  namespace direct {
    void compute_fortran(benchmark::State& state);
    void compute_cpp(benchmark::State& state);
  }
  namespace tangent {
    void compute_tapenade(benchmark::State& state);
    void compute_codipack(benchmark::State& state);
    void compute_adolc(benchmark::State& state);
    void compute_adept(benchmark::State& state);
  }
  namespace reverse {
    void compute_tapenade(benchmark::State& state);
    void compute_codipack(benchmark::State& state);
    void compute_adolc(benchmark::State& state);
    void compute_adept(benchmark::State& state);
  }
}

namespace roe_flux_case {
  namespace direct {
    void compute_fortran(benchmark::State& state);
    void compute_cpp(benchmark::State& state);
  }
  namespace tangent {
    void compute_tapenade(benchmark::State& state);
    void compute_codipack(benchmark::State& state);
    void compute_adolc(benchmark::State& state);
    void compute_adept(benchmark::State& state);
  }
  namespace reverse {
    void compute_tapenade(benchmark::State& state);
    void compute_codipack(benchmark::State& state);
    void compute_adolc(benchmark::State& state);
    void compute_adept(benchmark::State& state);
  }
}
