#include "ad_bench/benchmark/all.hpp"

#include "ad_bench/operation/all.hpp"

#include "ad_bench/sim_fields.hpp"
#include "ad_bench/centered_grad.hpp"
#include "ad_bench/roe_flux.hpp"

#include "codi.hpp"

#include <benchmark/benchmark.h>

namespace centered_grad_case {
  namespace reverse {
    using fields_codipack = sim_fields<codi::RealReverse,var_field_e,double,param_field_e>;

    fields_codipack initialize_codipack(int nb_cells, int nb_fict_cells) {
      fields_codipack flds(nb_cells,nb_fict_cells);
      for (int i=0; i<flds.size(); ++i) {
        flds(w)[i] = i;
      }
      return flds;
    }

    void compute_codipack(benchmark::State& state) {
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
      state.counters["lambda_w[nb_cells/2]"] = flds(w)[nb_cells/2].getGradient();
    }
  } // reverse
} // centered_grad_case



namespace roe_flux_case {
  namespace reverse {
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

    void compute_codipack(benchmark::State& state) {
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
      state.counters["lambda_rho[nb_cells/2]"] = flds(rho)[nb_cells/2].getGradient();
    }
  } // reverse
} //roe_flux_case
