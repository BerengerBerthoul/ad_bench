#pragma once

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
  }
  namespace tangent {
    enum var_field_e {
      w,
      dw,
      delta_w,
      delta_dw,
      nb_var_fields
    };
  }
  namespace reverse {
    enum var_field_e {
      w,
      dw,
      lambda_w,
      lambda_dw,
      nb_var_fields
    };
  }
} // centered_grad_case
