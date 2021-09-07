#include<vector>
#include <numeric>
#include <chrono>

#include "enzyme_functions.hpp"

#include <benchmark/benchmark.h>

using namespace functions;

void enzyme_centered_gradient(benchmark::State& state)
{
  /** 

     1D - CENTERED GRADIENT CASE (REVERSE ONLY)

  **/

  // Ghost cells and total size of mesh
  int gh = 2;
  int n_cell = state.range(0);
  int sz = n_cell+2*gh;

  // Vector allocation and pointer definition
  std::vector<double> w(sz);  double * p_w = w.data();
  std::vector<double> dw(sz); double * p_dw = dw.data();

  std::vector<double> d_w(sz);  double * p_d_w = d_w.data();
  std::vector<double> d_dw(sz); double * p_d_dw = d_dw.data();

  //deduce array type for instantiating CenteredGradient class 
  using p_type =typename std::remove_reference_t<decltype(*p_w)>;

  // Field initialization
  std::iota(p_w, p_w+sz, 0);
  std::iota(p_d_dw, p_d_dw+sz, 0);

  // Launch and timing
  for(auto _ : state)
  {
    //std::iota(p_d_dw, p_d_dw+sz, 0);
    CenteredGradient<p_type>::df(p_w, p_d_w, p_dw, p_d_dw, n_cell, gh);

  }

  state.counters["w_b[n_cell/2]"] = *(p_d_w+n_cell/2);
}

void enzyme_roe_flux(benchmark::State& state)
{
  /**

    3D - ROE FLUX CASE (REVERSE ONLY)

  **/

  // Ghost cells and total size of mesh
  int gh = 2;
  int n_cell = state.range(0);
  int sz = n_cell+2*gh;

  // Vector allocation and pointer definition
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
//
  //deduce array type for instantiating CenteredGradient class 
  using p_type =typename std::remove_reference_t<decltype(*rho)>;

  // Field initialization
  std::iota(surfx, surfx+sz, 0); std::iota(surfy, surfy+sz, 0); std::iota(surfz, surfz+sz, 0);

  std::iota(rho, rho+sz, 0); std::iota(velx, velx+sz, 0); std::iota(vely, vely+sz, 0);
  std::iota(velz, velz+sz, 0); std::iota(temp, temp+sz, 0);

  std::iota(flux1_b, flux1_b+sz, 0);
  std::iota(flux2_b, flux2_b+sz, 0);
  std::iota(flux4_b, flux4_b+sz, 0); 
  std::iota(flux3_b, flux3_b+sz, 0);
  std::iota(flux5_b, flux5_b+sz, 0);

  // Launch and timing
  for(auto _ : state)
  {
    RoeFlux<p_type>::df(rho, rho_b, velx, velx_b, vely, vely_b, velz, velz_b, temp, temp_b,
    /**surfx, surfy, surfz,**/ flux1, flux1_b, flux2, flux2_b, flux3, flux3_b, flux4, flux4_b, 
    flux5, flux5_b, n_cell, gh,
    surfx, surfy, surfz);
  }

  state.counters["rho_b[n_cell/2]"] = *(rho_b+n_cell/2);
}
