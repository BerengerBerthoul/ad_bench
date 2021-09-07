#include "operations.hpp"

#include <vector>
#include <iostream>
#include <chrono>
#include <utility>

#include <benchmark/benchmark.h>

void tapenade_centered_gradient(benchmark::State& state);
void tapenade_roe_flux(benchmark::State& state);
