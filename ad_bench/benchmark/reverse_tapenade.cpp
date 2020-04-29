#include "ad_bench/benchmark/all.hpp"
#include "ad_bench/operation/all.hpp"

#include "ad_bench/sim_fields.hpp"
#include "ad_bench/centered_grad.hpp"
#include "ad_bench/roe_flux.hpp"

#include <benchmark/benchmark.h>

namespace centered_grad_case {
  namespace reverse {
    using fields_tapenade = sim_fields<double,var_field_e,double,param_field_e>;
    fields_tapenade initialize_tapenade(int nb_cells, int nb_fict_cells) {
      fields_tapenade flds(nb_cells,nb_fict_cells);
      for (int i=0; i<flds.size(); ++i) {
        flds(w)      [i] = i;
      }
      return flds;
    }
    void compute_tapenade(benchmark::State& state) {
      int nb_cells = state.range(0);
      int nb_fict_cells = 2;
      auto flds = initialize_tapenade(nb_cells,nb_fict_cells);

      for (auto _ : state) {
        for(int i=0; i<flds.size(); ++i) {
          flds(lambda_dw)[i] = i;
        }

        centered_gradient_b_(
          flds(w), flds(lambda_w),
          flds(dw), flds(lambda_dw),
          nb_cells, nb_fict_cells
        );
      }
      state.counters["lambda_w[nb_cells/2]"] = flds(lambda_w)[nb_cells/2];
    }
  } // reverse
} // centered_grad_case



namespace roe_flux_case {
  namespace reverse {
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
      }
      return flds;
    }

    void compute_tapenade(benchmark::State& state) {
      int nb_cells = state.range(0);
      int nb_fict_cells = 2;
      auto flds = initialize_tapenade(nb_cells,nb_fict_cells);

      for (auto _ : state) {
        for(int i=0; i<flds.size(); ++i) {
          flds(lambda_f_rho)[i] = i;
          flds(lambda_f_u)[i] = i;
          flds(lambda_f_v)[i] = i;
          flds(lambda_f_w)[i] = i;
          flds(lambda_f_T)[i] = i;
        }

        roe_flux_b_(
          flds(rho), flds(lambda_rho), flds(u), flds(lambda_u), flds(v), flds(lambda_v), flds(w), flds(lambda_w), flds(T), flds(lambda_T), 
          flds(surf_x), flds(surf_y), flds(surf_z), 
          flds(f_rho), flds(lambda_f_rho), flds(f_u), flds(lambda_f_u), flds(f_v), flds(lambda_f_v), flds(f_w), flds(lambda_f_w), flds(f_T), flds(lambda_f_T), 
          nb_cells, nb_fict_cells
        );
      }
      state.counters["lambda_rho[nb_cells/2]"] = flds(lambda_rho)[nb_cells/2];
    }
  } // reverse
} //roe_flux_case
