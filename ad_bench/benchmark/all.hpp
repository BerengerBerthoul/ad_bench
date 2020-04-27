#pragma once

#include <utility>

#include <benchmark/benchmark.h>
namespace centered_grad_case {
  namespace direct {
    double compute_cpp(benchmark::State& state);
    double compute_fortran(benchmark::State& state);
  }
  namespace tangent {
    double compute_codipack(benchmark::State& state);
    double compute_tapenade(benchmark::State& state);
  }
  namespace reverse {
    double compute_codipack(benchmark::State& state);
    double compute_tapenade(benchmark::State& state);
  }
}

namespace roe_flux_case {
  namespace direct {
    double compute_cpp(benchmark::State& state);
    double compute_fortran(benchmark::State& state);
  }
  namespace tangent {
    double compute_codipack(benchmark::State& state);
    double compute_tapenade(benchmark::State& state);
  }
  namespace reverse {
    double compute_codipack(benchmark::State& state);
    double compute_tapenade(benchmark::State& state);
  }
}
