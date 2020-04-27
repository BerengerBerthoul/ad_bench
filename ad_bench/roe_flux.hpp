#pragma once

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
  }

  namespace tangent {
    enum var_field_e {
      rho,u,v,w,T,
      f_rho,f_u,f_v,f_w,f_T,
      delta_rho,delta_u,delta_v,delta_w,delta_T,
      delta_f_rho,delta_f_u,delta_f_v,delta_f_w,delta_f_T,
      nb_var_fields
    };
  }

  namespace reverse {
    enum var_field_e {
      rho,u,v,w,T,
      f_rho,f_u,f_v,f_w,f_T,
      lambda_rho,lambda_u,lambda_v,lambda_w,lambda_T,
      lambda_f_rho,lambda_f_u,lambda_f_v,lambda_f_w,lambda_f_T,
      nb_var_fields
    };
  }
} // roe_flux_case
