#pragma once

template<typename T>
__attribute__((noinline)) // because has to be like that with the Fortran version (when we call it from C++)
                          // and we want the benchmark to be fair
void centered_gradient(
  const T* __restrict__ w,
  T* __restrict__ dw,
  int n_cell, int gh)
{
  // NOTE: GCC applies SIMD with the correct compilation options (-ftree-vectorize -ftree-loop-vectorize) thanks to __restrict__
  for(int i=gh; i<gh+n_cell; ++i) {
    dw[i] = 0.5*(w[i+1] - w[i-1]);
  }
}
