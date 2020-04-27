#pragma once


// Class in charge of allocating/deallocating memory and give access to it
// There is a separation between variable fields (that can be differentiated)
// and parameter fields (used to compute the results, but not differentiated)
template<
  class variable_field_type, // e.g. float, double, or Operator Overloading type (codi::RealForward...)
  class variable_enum, // must be an enum with the name of the fields, and ending by "nb_var_fields"
  class param_field_type, // e.g. float, double,
  class param_enum // must be an enum with the name of the fields, and ending by "nb_param_fields"
>
class sim_fields {
  private:
    int sz;
    std::array< std::vector<variable_field_type> ,variable_enum::nb_var_fields  > var_fields;
    std::array< std::vector<   param_field_type> ,   param_enum::nb_param_fields> param_fields;
  public:
    sim_fields(int nb_cells, int nb_fict_cells)
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
