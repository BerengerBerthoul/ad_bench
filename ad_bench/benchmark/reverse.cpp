#include "ad_bench/benchmark/all.hpp"

#include <array>
#include <vector>

#include "ad_bench/benchmark/all.hpp"
#include "ad_bench/operation/all.hpp"

#include "ad_bench/sim_fields.hpp"
#include "ad_bench/centered_grad.hpp"
#include "ad_bench/roe_flux.hpp"

#include "codi.hpp"

#include <benchmark/benchmark.h>

namespace centered_grad_case {
  namespace reverse {
  // CoDiPack
    using fields_codipack = sim_fields<codi::RealReverse,var_field_e,double,param_field_e>;

    fields_codipack initialize_codipack(int nb_cells, int nb_fict_cells) {
      fields_codipack flds(nb_cells,nb_fict_cells);
      for (int i=0; i<flds.size(); ++i) {
        flds(w)[i] = i;
      }
      return flds;
    }

    double compute_codipack(benchmark::State& state) {
      int nb_cells = state.range(0);
      int nb_fict_cells = 2;
      auto flds = initialize_codipack(nb_cells,nb_fict_cells);
 
      // Configure tape
      codi::RealReverse::TapeType& tape = codi::RealReverse::getGlobalTape();

      // 1. begin recording
      tape.setActive();    

      // 2. apply operator
      for(int i=0; i<flds.size(); ++i) { tape.registerInput(flds(w)[i]); } // register inputs
      centered_gradient(flds(w), flds(dw), nb_cells, nb_fict_cells); // register function evaluation
      for(int i=0; i<flds.size(); ++i) { tape.registerOutput(flds(dw)[i]); } // register outputs
      
      // 3. stop recording
      tape.setPassive();

      for (auto _ : state) {
        // set the left vector
        for(int i=0; i<flds.size(); ++i) {
          flds(dw)[i].setGradient(double(i));
        }

        // evaluate the tape i.e. execute adjoint code
        tape.evaluate();
      }
      return flds(w)[nb_cells/2].getGradient();
    }

  // Tapenade
    using fields_tapenade = sim_fields<double,var_field_e,double,param_field_e>;
    fields_tapenade initialize_tapenade(int nb_cells, int nb_fict_cells) {
      fields_tapenade flds(nb_cells,nb_fict_cells);
      for (int i=0; i<flds.size(); ++i) {
        flds(w)      [i] = i;
      }
      return flds;
    }
    double compute_tapenade(benchmark::State& state) {
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
      return flds(lambda_w)[nb_cells/2];
    }
  } // reverse
} // centered_grad_case



namespace roe_flux_case {
  namespace reverse {
  // CoDiPack
    using fields_codipack = sim_fields<codi::RealReverse,var_field_e,double,param_field_e>;

    fields_codipack initialize_codipack(int nb_cells, int nb_fict_cells) {
      fields_codipack flds(nb_cells,nb_fict_cells);
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

    double compute_codipack(benchmark::State& state) {
      int nb_cells = state.range(0);
      int nb_fict_cells = 2;
      auto flds = initialize_codipack(nb_cells,nb_fict_cells);
 
      // Configure tape
      codi::RealReverse::TapeType& tape = codi::RealReverse::getGlobalTape();

      // 1. begin recording
      tape.setActive();    

      // 2. apply operator
      for(int i=0; i<flds.size(); ++i) { // register inputs
        tape.registerInput(flds(rho)[i]);
        tape.registerInput(flds(u)[i]);
        tape.registerInput(flds(v)[i]);
        tape.registerInput(flds(w)[i]);
        tape.registerInput(flds(T)[i]);
      }
      roe_flux( // register function evaluation
        flds(rho), flds(u), flds(v), flds(w), flds(T), 
        flds(surf_x), flds(surf_y), flds(surf_z), 
        flds(f_rho), flds(f_u), flds(f_v), flds(f_w), flds(f_T), 
        nb_cells, nb_fict_cells
      );
      for(int i=0; i<flds.size(); ++i) { // register outputs
        tape.registerOutput(flds(f_rho)[i]);
        tape.registerOutput(flds(f_u)[i]);
        tape.registerOutput(flds(f_v)[i]);
        tape.registerOutput(flds(f_w)[i]);
        tape.registerOutput(flds(f_T)[i]);
      }
      
      // 3. stop recording
      tape.setPassive();

      for (auto _ : state) {
        // set the left vector
        for(int i=0; i<flds.size(); ++i) {
          flds(f_rho)[i].setGradient(double(i));
          flds(f_u)[i].setGradient(double(i));
          flds(f_v)[i].setGradient(double(i));
          flds(f_w)[i].setGradient(double(i));
          flds(f_T)[i].setGradient(double(i));
        }
        // evaluate the tape i.e. execute adjoint code
        tape.evaluate();
      }
      return flds(rho)[nb_cells/2].getGradient();
    }

  // Tapenade
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

    double compute_tapenade(benchmark::State& state) {
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
      return flds(lambda_rho)[nb_cells/2];
    }
  } // reverse
} //roe_flux_case
