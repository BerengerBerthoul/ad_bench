#include "ad_bench/benchmark/all.hpp"

#include <array>
#include <vector>

#include "ad_bench/benchmark/all.hpp"
#include "ad_bench/operation/all.hpp"
#include <benchmark/benchmark.h>
#include <iostream>


template<
  class variable_field_type, class variable_enum,
  class param_field_type, class param_enum
>
class Sim_fields {
  private:
    int sz;
    std::array< std::vector<variable_field_type> ,variable_enum::nb_var_fields  > var_fields;
    std::array< std::vector<   param_field_type> ,   param_enum::nb_param_fields> param_fields;
  public:
    Sim_fields(int nb_cells, int nb_fict_cells)
      : sz(nb_cells+2*nb_fict_cells)
    {
      // allocate field memory
      for (auto& var_field : var_fields) {
        var_field = std::vector<variable_field_type>(sz);
      }
      for (auto& param_field : param_fields) {
        param_field = std::vector<param_field_type>(sz);
      }
    }

    int size() const { return sz; }

    variable_field_type* operator()(variable_enum i){ return var_fields[i].data(); };
    param_field_type* operator()(param_enum i){ return param_fields[i].data(); };
};

namespace centered_grad_case {
  enum param_field_e {
    nb_param_fields=0 // no parameter field needed to compute centered gradient
  };

  namespace direct {
    enum var_field_e {
      w,
      dw,
      nb_var_fields
    };
    using Fields = Sim_fields<double,var_field_e,double,param_field_e>;

    Fields initialize(int nb_cells, int nb_fict_cells) {
      Fields flds(nb_cells,nb_fict_cells);
      for (int i=0; i<flds.size(); ++i) {
        flds(w)[i] = i;
      }
      return flds;
    }

    double compute_cpp(benchmark::State& state) {
      int nb_cells = state.range(0);
      int nb_fict_cells = 2;
      auto flds = initialize(nb_cells,nb_fict_cells);

      for (auto _ : state) {
        centered_gradient(flds(w), flds(dw), nb_cells, nb_fict_cells);
      }
      return flds(dw)[nb_cells/2];
    }
    double compute_fortran(benchmark::State& state) {
      int nb_cells = state.range(0);
      int nb_fict_cells = 2;
      auto flds = initialize(nb_cells,nb_fict_cells);

      for (auto _ : state) {
        centered_gradient_(flds(w), flds(dw), nb_cells, nb_fict_cells);
      }
      return flds(dw)[nb_cells/2];
    }
  } // direct
} // centered_grad_case



namespace roe_flux_case {
  enum param_field_e {
    surf_x, surf_y, surf_z,
    nb_param_fields
  };

  namespace direct {
    enum var_field_e {
      rho,u,v,w,T,
      f_rho,f_u,f_v,f_w,f_T,
      nb_var_fields
    };
    using Fields = Sim_fields<double,var_field_e,double,param_field_e>;

    Fields initialize(int nb_cells, int nb_fict_cells) {
      Fields flds(nb_cells,nb_fict_cells);
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

    double compute_cpp(benchmark::State& state) {
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
      return flds(f_rho)[nb_cells/2];
    }
    double compute_fortran(benchmark::State& state) {
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
      return flds(f_rho)[nb_cells/2];
    }
  } // direct
} //roe_flux_case
