#include "ad_bench/benchmark/all.hpp"

#include "ad_bench/operation/all.hpp"

#include "ad_bench/sim_fields.hpp"
#include "ad_bench/centered_grad.hpp"
#include "ad_bench/roe_flux.hpp"

#include <adept_arrays.h>
using namespace adept;

#include <benchmark/benchmark.h>

namespace centered_grad_case {
  namespace tangent {

    void compute_adept(benchmark::State& state) {
      int nb_cells = state.range(0);
      int nb_fict_cells = 2;
      int sz = nb_cells+2*nb_fict_cells;

      Stack s;

      // Creation
      std::vector<adouble>  w_vec(sz);  adouble* w  = w_vec.data();
      std::vector<adouble>  dw_vec(sz); adouble* dw  = dw_vec.data();

      // Initialize w
      for (int i=0; i<sz; ++i) {
         w[i] = i; // NOTE: unused for this particular operator
         w[i].set_gradient(double(i));
      }

      s.new_recording();

      // 2. apply operator
      centered_gradient(w, dw, nb_cells, nb_fict_cells);

      for (auto _ : state) {
        for (int i=0;i<sz;++i){
         //w[i] = i;
         w[i].set_gradient(double(i));
        }
        s.forward();
      }

      double Z = dw[nb_cells/2].get_gradient();
      state.counters["delta_dw[nb_cells/2]"] = Z;
    }
  } // tangent
} // centered_grad_case



namespace roe_flux_case {
  namespace tangent {
    void compute_adept(benchmark::State& state) {
      int nb_cells = state.range(0);
      int nb_fict_cells = 2;
      auto sz = nb_cells+2*nb_fict_cells;

      // Creation
      std::vector<double>      surfx_vec(sz); double*      surfx = surfx_vec.data();
      std::vector<double>      surfy_vec(sz); double*      surfy = surfy_vec.data();
      std::vector<double>      surfz_vec(sz); double*      surfz = surfz_vec.data();

      // Initialize and begin recording tape
      for(int i=0; i<sz; ++i){
        surfx[i] = i;
        surfy[i] = i;
        surfz[i] = i;
      }

      Stack s;

      std::vector<adouble>   rho_vec(sz); adouble*   rho =   rho_vec.data();
      std::vector<adouble>  velx_vec(sz); adouble*  velx =  velx_vec.data();
      std::vector<adouble>  vely_vec(sz); adouble*  vely =  vely_vec.data();
      std::vector<adouble>  velz_vec(sz); adouble*  velz =  velz_vec.data();
      std::vector<adouble>  temp_vec(sz); adouble*  temp =  temp_vec.data();
      std::vector<adouble> flux1_vec(sz); adouble* flux1 = flux1_vec.data();
      std::vector<adouble> flux2_vec(sz); adouble* flux2 = flux2_vec.data();
      std::vector<adouble> flux3_vec(sz); adouble* flux3 = flux3_vec.data();
      std::vector<adouble> flux4_vec(sz); adouble* flux4 = flux4_vec.data();
      std::vector<adouble> flux5_vec(sz); adouble* flux5 = flux5_vec.data();

      for(int i=0; i<sz; ++i){
        rho  [i] = i;
        velx [i] = i;
        vely [i] = i;
        velz [i] = i;
        temp [i] = i;

        rho [i].set_gradient(double(i));
        velx[i].set_gradient(double(i));
        vely[i].set_gradient(double(i));
        velz[i].set_gradient(double(i));
        temp[i].set_gradient(double(i));
      }

      s.new_recording();

      // Apply operator
      roe_flux( // register function evaluation
        rho, velx, vely, velz, temp,
        surfx, surfy, surfz,
        flux1, flux2, flux3, flux4, flux5,
        nb_cells, nb_fict_cells
      );

      for (auto _ : state) {
        for(int i=0; i<sz; ++i){
          rho[i].set_gradient(double(i));
          velx[i].set_gradient(double(i));
          vely[i].set_gradient(double(i));
          velz[i].set_gradient(double(i));
          temp[i].set_gradient(double(i));
        }
      s.forward();
      }
      double Z = flux1[nb_cells/2].get_gradient();
      state.counters["delta_f_rho[nb_cells/2]"] = Z ;
    }
  } // tangent
} //roe_flux_case
