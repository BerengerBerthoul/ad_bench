#include "ad_bench/benchmark/all.hpp"

#include "ad_bench/operation/all.hpp"

#include "ad_bench/sim_fields.hpp"
#include "ad_bench/centered_grad.hpp"
#include "ad_bench/roe_flux.hpp"

#include <adolc/adtl.h>

#include <benchmark/benchmark.h>

namespace centered_grad_case {
  namespace tangent {
    using fields_adolc = sim_fields<adtl::adouble,var_field_e,double,param_field_e>;

    fields_adolc initialize_adolc(int nb_cells, int nb_fict_cells) {
      fields_adolc flds(nb_cells,nb_fict_cells);
      for (int i=0; i<flds.size(); ++i) {
        flds(w)[i] = i;
        flds(w)[i].setADValue(0,i);
      }
      return flds;
    }

    void compute_adolc(benchmark::State& state) {
      int nb_cells = state.range(0);
      int nb_fict_cells = 2;
      auto flds = initialize_adolc(nb_cells,nb_fict_cells);

      for (auto _ : state) {
        centered_gradient(flds(w), flds(dw), nb_cells, nb_fict_cells);
      }
      state.counters["delta_dw[nb_cells/2]"] = flds(dw)[nb_cells/2].getADValue()[0];
    }
  } // tangent
} // centered_grad_case



namespace roe_flux_case {
  namespace tangent {
    using fields_adolc = sim_fields<adtl::adouble,var_field_e,double,param_field_e>;

    fields_adolc initialize_adolc(int nb_cells, int nb_fict_cells) {
      fields_adolc flds(nb_cells,nb_fict_cells);
      for (int i=0; i<flds.size(); ++i) {
        flds(surf_x)[i] = i;
        flds(surf_y)[i] = i;
        flds(surf_z)[i] = i;

        flds(rho)[i] = i;
        flds(u)[i] = i;
        flds(v)[i] = i;
        flds(w)[i] = i;
        flds(T)[i] = i;

        flds(rho)[i].setADValue(0,i);
        flds(u)[i]  .setADValue(0,i);
        flds(v)[i]  .setADValue(0,i);
        flds(w)[i]  .setADValue(0,i);
        flds(T)[i]  .setADValue(0,i);
      }
      return flds;
    }

    void compute_adolc(benchmark::State& state) {
      int nb_cells = state.range(0);
      int nb_fict_cells = 2;
      auto flds = initialize_adolc(nb_cells,nb_fict_cells);

      for (auto _ : state) {
        roe_flux(
          flds(rho), flds(u), flds(v), flds(w), flds(T), 
          flds(surf_x), flds(surf_y), flds(surf_z), 
          flds(f_rho), flds(f_u), flds(f_v), flds(f_w), flds(f_T), 
          nb_cells, nb_fict_cells
        );
      }
      state.counters["delta_f_rho[nb_cells/2]"] = flds(f_rho)[nb_cells/2].getADValue()[0];
    }
  } // tangent
} //roe_flux_case
