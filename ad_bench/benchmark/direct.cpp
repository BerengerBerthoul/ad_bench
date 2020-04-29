#include "ad_bench/benchmark/all.hpp"

#include "ad_bench/operation/all.hpp"

#include "ad_bench/sim_fields.hpp"
#include "ad_bench/centered_grad.hpp"
#include "ad_bench/roe_flux.hpp"

#include <benchmark/benchmark.h>
#include <gtest/gtest.h>

namespace centered_grad_case {
  namespace direct {
    using fields = sim_fields<double,var_field_e,double,param_field_e>;

    fields initialize(int nb_cells, int nb_fict_cells) {
      fields flds(nb_cells,nb_fict_cells);
      for (int i=0; i<flds.size(); ++i) {
        flds(w)[i] = i;
      }
      return flds;
    }

    void compute_cpp(benchmark::State& state) {
      int nb_cells = state.range(0);
      int nb_fict_cells = 2;
      auto flds = initialize(nb_cells,nb_fict_cells);

      for (auto _ : state) {
        centered_gradient(flds(w), flds(dw), nb_cells, nb_fict_cells);
      }
      state.counters["dw[nb_cells/2]"] = flds(dw)[nb_cells/2];
    }
    void compute_fortran(benchmark::State& state) {
      int nb_cells = state.range(0);
      int nb_fict_cells = 2;
      auto flds = initialize(nb_cells,nb_fict_cells);

      for (auto _ : state) {
        centered_gradient_(flds(w), flds(dw), nb_cells, nb_fict_cells);
      }
      state.counters["dw[nb_cells/2]"] = flds(dw)[nb_cells/2];
    }
  } // direct
} // centered_grad_case



namespace roe_flux_case {
  namespace direct {
    using fields = sim_fields<double,var_field_e,double,param_field_e>;

    fields initialize(int nb_cells, int nb_fict_cells) {
      fields flds(nb_cells,nb_fict_cells);
      for (int i=0; i<flds.size(); ++i) {
        flds(surf_x)[i] = i;
        flds(surf_y)[i] = i;
        flds(surf_z)[i] = i;

        flds(rho)[i] = i;
        flds(u)[i] = i;
        flds(v)[i] = i;
        flds(w)[i] = i;
        flds(T)[i] = i;
      }
      return flds;
    }

    void compute_cpp(benchmark::State& state) {
      int nb_cells = state.range(0);
      int nb_fict_cells = 2;
      auto flds = initialize(nb_cells,nb_fict_cells);

      for (auto _ : state) {
        roe_flux(
          flds(rho), flds(u), flds(v), flds(w), flds(T), 
          flds(surf_x), flds(surf_y), flds(surf_z), 
          flds(f_rho), flds(f_u), flds(f_v), flds(f_w), flds(f_T), 
          nb_cells, nb_fict_cells
        );
      }
      state.counters["f_rho[nb_cells/2]"] = flds(f_rho)[nb_cells/2];
    }
    void compute_fortran(benchmark::State& state) {
      int nb_cells = state.range(0);
      int nb_fict_cells = 2;
      auto flds = initialize(nb_cells,nb_fict_cells);

      for (auto _ : state) {
        roe_flux_(
          flds(rho), flds(u), flds(v), flds(w), flds(T), 
          flds(surf_x), flds(surf_y), flds(surf_z), 
          flds(f_rho), flds(f_u), flds(f_v), flds(f_w), flds(f_T), 
          nb_cells, nb_fict_cells
        );
      }
      state.counters["f_rho[nb_cells/2]"] = flds(f_rho)[nb_cells/2];
    }
  } // direct
} //roe_flux_case
