#include "ad_bench/benchmark/all.hpp"

#include "ad_bench/operation/all.hpp"

#include "ad_bench/sim_fields.hpp"
#include "ad_bench/centered_grad.hpp"
#include "ad_bench/roe_flux.hpp"

#include <benchmark/benchmark.h>

namespace centered_grad_case {
  namespace tangent {
    using fields_tapenade = sim_fields<double,var_field_e,double,param_field_e>;
    fields_tapenade initialize_tapenade(int nb_cells, int nb_fict_cells) {
      fields_tapenade flds(nb_cells,nb_fict_cells);
      for (int i=0; i<flds.size(); ++i) {
        flds(w)      [i] = i;
        flds(delta_w)[i] = i;
      }
      return flds;
    }
    void compute_tapenade(benchmark::State& state) {
      int nb_cells = state.range(0);
      int nb_fict_cells = 2;
      auto flds = initialize_tapenade(nb_cells,nb_fict_cells);

      for (auto _ : state) {
        centered_gradient_d_(
          flds(w), flds(delta_w),
          flds(dw), flds(delta_dw),
          nb_cells, nb_fict_cells
        );
      }
      state.counters["delta_dw[nb_cells/2]"] = flds(delta_dw)[nb_cells/2];
    }
  } // tangent
} // centered_grad_case



namespace roe_flux_case {
  namespace tangent {
    using fields_tapenade = sim_fields<double,var_field_e,double,param_field_e>;

    fields_tapenade initialize_tapenade(int nb_cells, int nb_fict_cells) {
      fields_tapenade flds(nb_cells,nb_fict_cells);
      for (int i=0; i<flds.size(); ++i) {
        flds(surf_x)[i] = i;
        flds(surf_y)[i] = i;
        flds(surf_z)[i] = i;

        flds(rho)[i] = i;
        flds(u)[i] = i;
        flds(v)[i] = i;
        flds(w)[i] = i;
        flds(T)[i] = i;

        flds(delta_rho)[i] = i;
        flds(delta_u)[i] = i;
        flds(delta_v)[i] = i;
        flds(delta_w)[i] = i;
        flds(delta_T)[i] = i;
      }
      return flds;
    }

    void compute_tapenade(benchmark::State& state) {
      int nb_cells = state.range(0);
      int nb_fict_cells = 2;
      auto flds = initialize_tapenade(nb_cells,nb_fict_cells);

      for (auto _ : state) {
        roe_flux_d_(
          flds(rho), flds(delta_rho), flds(u), flds(delta_u), flds(v), flds(delta_v), flds(w), flds(delta_w), flds(T), flds(delta_T), 
          flds(surf_x), flds(surf_y), flds(surf_z), 
          flds(f_rho), flds(delta_f_rho), flds(f_u), flds(delta_f_u), flds(f_v), flds(delta_f_v), flds(f_w), flds(delta_f_w), flds(f_T), flds(delta_f_T), 
          nb_cells, nb_fict_cells
        );
      }
      state.counters["delta_f_rho[nb_cells/2]"] = flds(delta_f_rho)[nb_cells/2];
    }
  } // tangent
} //roe_flux_case
