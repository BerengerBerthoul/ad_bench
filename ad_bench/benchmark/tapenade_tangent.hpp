#pragma once

#include <vector>
#include <iostream>
#include <chrono>

#include "ad_bench/operation/all.hpp"
#include "codi.hpp"
using codi::RealForward;

inline
double benchmark_centered_gradient_tapenade_tangent(int n_iter, int n_cell) {
  int gh = 2;
  int sz = n_cell+2*gh;
  
  // Creation
  std::vector<double>    w_vec(sz); double* w    =    w_vec.data();
  std::vector<double>  w_d_vec(sz); double* w_d  =  w_d_vec.data();
  std::vector<double>   dw_vec(sz); double* dw   =   dw_vec.data();
  std::vector<double> dw_d_vec(sz); double* dw_d = dw_d_vec.data();
  
  // Initialize
  for (int i=0; i<sz; ++i) {
      w[i] = i;
    w_d[i] = i;
  }
  
  // Compute
  auto start = std::chrono::system_clock::now();
  for(int iter=0; iter < n_iter; ++iter) {
    centered_gradient_d_(w, w_d, dw, dw_d, n_cell, gh);
  }
  auto end = std::chrono::system_clock::now();
  
  std::cout << "gradient dw: " << dw_d[n_cell/2] << std::endl;
  return std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
}

inline
double benchmark_roe_flux_tapenade_tangent(int n_iter, int n_cell) {
  int gh = 2;
  int sz = n_cell+2*gh;
  
  // Creation
  std::vector<double>     rho_vec(sz); double*   rho   =     rho_vec.data();
  std::vector<double>    velx_vec(sz); double*  velx   =    velx_vec.data();
  std::vector<double>    vely_vec(sz); double*  vely   =    vely_vec.data();
  std::vector<double>    velz_vec(sz); double*  velz   =    velz_vec.data();
  std::vector<double>    temp_vec(sz); double*  temp   =    temp_vec.data();
  std::vector<double>   flux1_vec(sz); double* flux1   =   flux1_vec.data();
  std::vector<double>   flux2_vec(sz); double* flux2   =   flux2_vec.data();
  std::vector<double>   flux3_vec(sz); double* flux3   =   flux3_vec.data();
  std::vector<double>   flux4_vec(sz); double* flux4   =   flux4_vec.data();
  std::vector<double>   flux5_vec(sz); double* flux5   =   flux5_vec.data();

  std::vector<double>   rho_d_vec(sz); double*   rho_d =   rho_d_vec.data();
  std::vector<double>  velx_d_vec(sz); double*  velx_d =  velx_d_vec.data();
  std::vector<double>  vely_d_vec(sz); double*  vely_d =  vely_d_vec.data();
  std::vector<double>  velz_d_vec(sz); double*  velz_d =  velz_d_vec.data();
  std::vector<double>  temp_d_vec(sz); double*  temp_d =  temp_d_vec.data();
  std::vector<double> flux1_d_vec(sz); double* flux1_d = flux1_d_vec.data();
  std::vector<double> flux2_d_vec(sz); double* flux2_d = flux2_d_vec.data();
  std::vector<double> flux3_d_vec(sz); double* flux3_d = flux3_d_vec.data();
  std::vector<double> flux4_d_vec(sz); double* flux4_d = flux4_d_vec.data();
  std::vector<double> flux5_d_vec(sz); double* flux5_d = flux5_d_vec.data();

  std::vector<double>   surfx_vec(sz); double* surfx   =   surfx_vec.data();
  std::vector<double>   surfy_vec(sz); double* surfy   =   surfy_vec.data();
  std::vector<double>   surfz_vec(sz); double* surfz   =   surfz_vec.data();
  
  // Initialize fields
  for(int i=0; i < sz; ++i){
    rho  [i] = i; 
    velx [i] = i; 
    vely [i] = i; 
    velz [i] = i; 
    temp [i] = i; 

    surfx[i] = i; 
    surfy[i] = i; 
    surfz[i] = i; 

     rho_d[i] = i;
    velx_d[i] = i;
    vely_d[i] = i;
    velz_d[i] = i;
    temp_d[i] = i;
  }
  
  // Compute
  auto start = std::chrono::system_clock::now();
  for (int iter=0; iter<n_iter; ++iter) {
    roe_flux_d_(
      rho, rho_d, velx, velx_d, vely, vely_d, velz, velz_d, temp, temp_d, 
      surfx, surfy, surfz, 
      flux1, flux1_d, flux2, flux2_d, flux3, flux3_d, flux4, flux4_d, flux5, flux5_d, 
      n_cell, gh
    );
  }
  auto end = std::chrono::system_clock::now();
  
  std::cout << "value: " << flux1[n_cell/2] << "\n";
  std::cout << "gradient: " << flux1_d[n_cell/2] << "\n";
  return std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
}
