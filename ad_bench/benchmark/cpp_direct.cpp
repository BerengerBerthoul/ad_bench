#include "ad_bench/benchmark/all.hpp"

#include <vector>
#include <iostream>
#include <chrono>

#include "ad_bench/benchmark/all.hpp"
#include "ad_bench/operation/all.hpp"

std::pair<double,double> benchmark_centered_gradient_cpp(int n_iter, int n_cell) {
  int gh = 2;
  int sz = n_cell+2*gh;
  
  // Creation
  std::vector<double>  w_vec(sz); double* w = w_vec.data();
  std::vector<double> dw_vec(sz); double* dw = dw_vec.data();
  
  // Initialize
  for (int i=0; i<sz; ++i) {
    w[i] = i;
  }
  
  // Compute
  double cnt = 0;
  auto start = std::chrono::system_clock::now();
  for(int iter=0; iter<n_iter; ++iter) {
    centered_gradient(w, dw, n_cell, gh);
    cnt += dw[n_cell/2];
  }
  auto end = std::chrono::system_clock::now();
  
  return std::make_pair(
    dw[n_cell/2]+cnt,
    std::chrono::duration<double>(end-start).count()
  );
}

std::pair<double,double> benchmark_roe_flux_cpp(int n_iter, int n_cell) {
  int gh = 2;
  int sz = n_cell+2*gh;
  
  // Creation
  std::vector<double> surfx_vec(sz); double* surfx = surfx_vec.data();
  std::vector<double> surfy_vec(sz); double* surfy = surfy_vec.data();
  std::vector<double> surfz_vec(sz); double* surfz = surfz_vec.data();

  std::vector<double>   rho_vec(sz); double*   rho =   rho_vec.data();
  std::vector<double>  velx_vec(sz); double*  velx =  velx_vec.data();
  std::vector<double>  vely_vec(sz); double*  vely =  vely_vec.data();
  std::vector<double>  velz_vec(sz); double*  velz =  velz_vec.data();
  std::vector<double>  temp_vec(sz); double*  temp =  temp_vec.data();

  std::vector<double> flux1_vec(sz); double* flux1 = flux1_vec.data();
  std::vector<double> flux2_vec(sz); double* flux2 = flux2_vec.data();
  std::vector<double> flux3_vec(sz); double* flux3 = flux3_vec.data();
  std::vector<double> flux4_vec(sz); double* flux4 = flux4_vec.data();
  std::vector<double> flux5_vec(sz); double* flux5 = flux5_vec.data();
  
  // Initialize
  for(int i=0; i<sz; ++i){
    surfx[i] = i;
    surfy[i] = i;
    surfz[i] = i;

    rho  [i] = i;
    velx [i] = i;
    vely [i] = i;
    velz [i] = i;
    temp [i] = i;
  }
  
  // Compute
  auto start = std::chrono::system_clock::now();
  for (int iter=0; iter<n_iter; ++iter) {
    roe_flux(
      rho, velx, vely, velz, temp, 
      surfx, surfy, surfz, 
      flux1, flux2, flux3, flux4, flux5, 
      n_cell, gh
    );
  }
  auto end = std::chrono::system_clock::now();
  
  return std::make_pair(
    flux1[n_cell/2],
    std::chrono::duration<double>(end-start).count()
  );
}
