#include "operations.hpp"

#include <vector>
#include <iostream>
#include <chrono>

#include "tapenade_reverse.hpp"

#include <benchmark/benchmark.h>

//std::pair<double,double> benchmark_centered_gradient_tapenade_reverse(int n_iter, int n_cell) {
void tapenade_centered_gradient(benchmark::State& state)
{
  //init 
  int gh = 2;
  int n_cell = state.range(0);
  int sz = n_cell+2*gh;

  // Creation
  std::vector<double>    w_vec(sz); double* w    =    w_vec.data();
  std::vector<double>   dw_vec(sz); double* dw   =   dw_vec.data();
  std::vector<double>  w_b_vec(sz); double* w_b  =  w_b_vec.data();
  std::vector<double> dw_b_vec(sz); double* dw_b = dw_b_vec.data();
  
  // Initialize
  for (int i=0; i<sz; ++i) {
       w[i] = i; // NOTE: unused for this particular operator
  }
  
  // Compute
  for(auto _ : state) 
  {
    // Reset array
    for(int i=0; i<sz; ++i) 
    {
      dw_b[i] = i;
    }

    centered_gradient_b_(w, w_b, dw, dw_b, n_cell, gh);
  }

  state.counters["w_b[n_cell/2]"] = *(w_b+n_cell/2); 
}

void tapenade_roe_flux(benchmark::State& state)
{
  int gh = 2;
  int n_cell = state.range(0);
  int sz = n_cell+2*gh;
  
  // Creation
  std::vector<double>   surfx_vec(sz); double* surfx   =   surfx_vec.data();
  std::vector<double>   surfy_vec(sz); double* surfy   =   surfy_vec.data();
  std::vector<double>   surfz_vec(sz); double* surfz   =   surfz_vec.data();

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

  std::vector<double>   rho_b_vec(sz); double*   rho_b =   rho_b_vec.data();
  std::vector<double>  velx_b_vec(sz); double*  velx_b =  velx_b_vec.data();
  std::vector<double>  vely_b_vec(sz); double*  vely_b =  vely_b_vec.data();
  std::vector<double>  velz_b_vec(sz); double*  velz_b =  velz_b_vec.data();
  std::vector<double>  temp_b_vec(sz); double*  temp_b =  temp_b_vec.data();
  std::vector<double> flux1_b_vec(sz); double* flux1_b = flux1_b_vec.data();
  std::vector<double> flux2_b_vec(sz); double* flux2_b = flux2_b_vec.data();
  std::vector<double> flux3_b_vec(sz); double* flux3_b = flux3_b_vec.data();
  std::vector<double> flux4_b_vec(sz); double* flux4_b = flux4_b_vec.data();
  std::vector<double> flux5_b_vec(sz); double* flux5_b = flux5_b_vec.data();
  
  // Initialize fields
  for(int i=0; i < sz; ++i){
    surfx[i] = i; 
    surfy[i] = i; 
    surfz[i] = i; 

    rho [i] = i; 
    velx[i] = i; 
    vely[i] = i; 
    velz[i] = i; 
    temp[i] = i; 
  }
  
  // Compute
  for (auto _ : state) 
  {
    // Reset array
    for(int i=0; i<sz; ++i) 
    {
      flux1_b[i] = i;
      flux2_b[i] = i;
      flux3_b[i] = i;
      flux4_b[i] = i;
      flux5_b[i] = i;
    }

    roe_flux_b_(
      rho, rho_b, velx, velx_b, vely, vely_b, velz, velz_b, temp, temp_b, 
      surfx, surfy, surfz, 
      flux1, flux1_b, flux2, flux2_b, flux3, flux3_b, flux4, flux4_b, flux5, flux5_b, 
      n_cell, gh
    );
  }
  
  state.counters["rho_b[n_cell/2]"] = *(rho_b+n_cell/2);
}
