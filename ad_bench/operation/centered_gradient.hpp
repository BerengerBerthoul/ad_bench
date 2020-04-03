#pragma once

template<typename T>
void centered_gradient(const T* w, T* dw, int n_cell, int gh) {
  for(int i=gh; i<gh+n_cell; ++i) {
    dw[i] = 0.5*(w[i+1] - w[i-1]);
  }
}
