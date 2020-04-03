#pragma once

#include <vector>
#include <iostream>
#include <chrono>

#include "ad_bench/operation/all.hpp"
#include "codi.hpp"
using codi::RealForward;

inline
double benchmark_centered_gradient_codipack_tangent(int n_iter, int n_cell) {
  int gh = 2;
  int sz = n_cell+2*gh;
  
  // Creation
  std::vector<RealForward>  w_vec(sz); RealForward* w = w_vec.data();
  std::vector<RealForward> dw_vec(sz); RealForward* dw = dw_vec.data();
  
  // Initialize
  for (int i=0; i<sz; ++i) {
    w[i] = i;
    w[i].setGradient(i);
  }
  
  // Compute
  auto start = std::chrono::system_clock::now();
  for(int iter=0; iter<n_iter; ++iter) {
    centered_gradient(w, dw, n_cell, gh);
  }
  auto end = std::chrono::system_clock::now();
  
  std::cout << "gradient dw: " << dw[n_cell/2].getGradient() << "\t";
  return std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
}

inline
double benchmark_roe_flux_codipack_tangent(int n_iter, int n_cell) {
  int gh = 2;
  int sz = n_cell+2*gh;
  
  // Creation
  std::vector<double>      surfx_vec(sz); double*      surfx = surfx_vec.data();
  std::vector<double>      surfy_vec(sz); double*      surfy = surfy_vec.data();
  std::vector<double>      surfz_vec(sz); double*      surfz = surfz_vec.data();

  std::vector<RealForward>   rho_vec(sz); RealForward*   rho =   rho_vec.data();
  std::vector<RealForward>  velx_vec(sz); RealForward*  velx =  velx_vec.data();
  std::vector<RealForward>  vely_vec(sz); RealForward*  vely =  vely_vec.data();
  std::vector<RealForward>  velz_vec(sz); RealForward*  velz =  velz_vec.data();
  std::vector<RealForward>  temp_vec(sz); RealForward*  temp =  temp_vec.data();
  std::vector<RealForward> flux1_vec(sz); RealForward* flux1 = flux1_vec.data();
  std::vector<RealForward> flux2_vec(sz); RealForward* flux2 = flux2_vec.data();
  std::vector<RealForward> flux3_vec(sz); RealForward* flux3 = flux3_vec.data();
  std::vector<RealForward> flux4_vec(sz); RealForward* flux4 = flux4_vec.data();
  std::vector<RealForward> flux5_vec(sz); RealForward* flux5 = flux5_vec.data();
  
  // Initialize fields
  for(int i=0; i<sz; ++i){
    surfx[i] = i; 
    surfy[i] = i; 
    surfz[i] = i; 

    rho  [i] = i; 
    velx [i] = i; 
    vely [i] = i; 
    velz [i] = i; 
    temp [i] = i; 

    rho [i].setGradient(i);
    velx[i].setGradient(i);
    vely[i].setGradient(i);
    velz[i].setGradient(i);
    temp[i].setGradient(i);
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
  
  std::cout << "gradient flux rho: " << flux1[n_cell/2].getGradient() << "\t";
  return std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
}
