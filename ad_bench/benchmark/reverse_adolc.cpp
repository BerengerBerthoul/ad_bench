#include "ad_bench/benchmark/all.hpp"

#include "ad_bench/operation/all.hpp"

#include "ad_bench/sim_fields.hpp"
#include "ad_bench/centered_grad.hpp"
#include "ad_bench/roe_flux.hpp"

#include "adolc/adolc.h"

#include <benchmark/benchmark.h>

namespace centered_grad_case {
  namespace reverse {
    using fields_adolc = sim_fields<double,var_field_e,double,param_field_e>;

    void compute_adolc(benchmark::State& state) {
    // init
      int nb_cells = state.range(0);
      int nb_fict_cells = 2;

      fields_adolc flds(nb_cells,nb_fict_cells);
      int sz = flds.size();

      std::vector<adouble>  w_tape_vec(sz); adouble*  w_tape =  w_tape_vec.data();
      std::vector<adouble> dw_tape_vec(sz); adouble* dw_tape = dw_tape_vec.data();
      std::vector<double>   y_tape_vec(sz); double *  y_tape =  y_tape_vec.data();
     
    // recording
      int tape_tag_id = 1;
      trace_on(tape_tag_id);
     
      // 1. record initialization
      for (int i=0; i<sz; ++i) {
         w_tape[i] <<= i; // NOTE: unused for this particular operator
      }

      // 2. apply operator
      centered_gradient(w_tape, dw_tape, nb_cells, nb_fict_cells); // register function evaluation
      
      // 3. ???
      for (int i=0; i<sz; ++i) {
        dw_tape[i] >>= y_tape[i];
      }
      trace_off();  

      // 4. ???
      for (int i=0; i<sz; ++i) {
         w_tape[i] >>= flds(w)[i];
      }

      for (auto _ : state) {
        // set the left vector
         for(int i=0; i<sz; ++i) {
           flds(lambda_dw)[i] = double(i);
         }

        // Compute vector-jacobian product
        vec_jac(tape_tag_id,sz,sz,0,flds(w),flds(lambda_dw),flds(lambda_w));
      }
      state.counters["lambda_w[nb_cells/2]"] = flds(lambda_w)[nb_cells/2];
    }
  } // reverse
} // centered_grad_case



namespace roe_flux_case {
  namespace reverse {
    void compute_adolc(benchmark::State& state) {
      int nb_cells = state.range(0);
      int nb_fict_cells = 2;
      int sz = nb_cells+2*nb_fict_cells;

      int tape_tag_id = 1;

      int ind_dim = 5*sz;
      int dep_dim = 5*sz;
      
      // Creation
      std::vector<double>      surfx_vec(sz); double*      surfx = surfx_vec.data();
      std::vector<double>      surfy_vec(sz); double*      surfy = surfy_vec.data();
      std::vector<double>      surfz_vec(sz); double*      surfz = surfz_vec.data();

      std::vector<adouble>   rho_vec(sz); adouble*   rho =   rho_vec.data();
      std::vector<adouble>  velx_vec(sz); adouble*  velx =  velx_vec.data();
      std::vector<adouble>  vely_vec(sz); adouble*  vely =  vely_vec.data();
      std::vector<adouble>  velz_vec(sz); adouble*  velz =  velz_vec.data();
      std::vector<adouble>  temp_vec(sz); adouble*  temp =  temp_vec.data();
      std::vector<adouble> flux1_vec(sz); adouble* flux1 = flux1_vec.data();
      std::vector<adouble> flux2_vec(sz); adouble* flux2 = flux2_vec.data();
      std::vector<adouble> flux3_vec(sz); adouble* flux3 = flux3_vec.data();
      std::vector<adouble> flux4_vec(sz); adouble* flux4 = flux4_vec.data();
      std::vector<adouble> flux5_vec(sz); adouble* flux5 = flux5_vec.data();
      std::vector<double> fx1_vec(sz); double* fx1 = fx1_vec.data();
      std::vector<double> fx2_vec(sz); double* fx2 = fx2_vec.data();
      std::vector<double> fx3_vec(sz); double* fx3 = fx3_vec.data();
      std::vector<double> fx4_vec(sz); double* fx4 = fx4_vec.data();
      std::vector<double> fx5_vec(sz); double* fx5 = fx5_vec.data(); 

      std::vector<double> TMP_VEC (ind_dim); double* TMP = TMP_VEC.data();

      // Initialize and begin recording tape
      for(int i=0; i<sz; ++i){
        surfx[i] = i; 
        surfy[i] = i; 
        surfz[i] = i; 
      }

      for (int j=0;j<5;j++){
        for(int i=0; i<sz; ++i)
          TMP[j*sz+i] = i;
        }
      
      // begin recording the tape
      trace_on(tape_tag_id);
     
      // store data contigously for each vector of independent variables on the tape
      // TODO: see if this can be improved(advector?)
      for(int i=0; i<sz; ++i)
        rho  [i] <<= i; 
      for(int i=0; i<sz; ++i)
        velx [i] <<= i; 
      for(int i=0; i<sz; ++i)
        vely [i] <<= i; 
      for(int i=0; i<sz; ++i)
        velz [i] <<= i; 
      for(int i=0; i<sz; ++i)
        temp [i] <<= i; 
     

      // Apply operator
      roe_flux( // register function evaluation
        rho, velx, vely, velz, temp, 
        surfx, surfy, surfz, 
        flux1, flux2, flux3, flux4, flux5, 
        nb_cells, nb_fict_cells
      );

      // store data contigously for each vector of dependent variables on the tape
      // TODO: see if this can be improved(advector?)
      for(int i=0; i<sz; ++i) // register outputs
        flux1[i]>>=fx1[i];
      for(int i=0; i<sz; ++i)
        flux2[i]>>=fx2[i];
      for(int i=0; i<sz; ++i)
        flux3[i]>>=fx3[i];
      for(int i=0; i<sz; ++i)
        flux4[i]>>=fx4[i];
      for(int i=0; i<sz; ++i)
        flux5[i]>>=fx5[i];
      
      
      // 3. stop recording the tape
      trace_off();  

      // Vectors to use in calculations involving the tape
      std::vector<double>  X_vec(ind_dim);  double*  X =  X_vec.data();
      std::vector<double>  dY_vec(ind_dim); double*  dY = dY_vec.data();
      std::vector<double>  Z_vec(dep_dim);  double*  Z =  Z_vec.data();

      // Vector initialization
      X=TMP;

    // COMM : Alternative use of functions invloving tape
    // COMM : left here for reference

    //  double** ZPP, **UPP, **XPP, **YPP, *YP;
    //  int p = 1;
    //
    //
    //  ZPP    = new double* [p];
    //  for (int i=0; i<p; i++)
    //    ZPP[i] = new double  [ind_dim];
    //
    //  UPP    = new double* [p];
    //  for (int i=0; i<p; i++){
    //    UPP[i] = new double  [dep_dim];
    //    for(int jj=0;jj<dep_dim;++jj){ 
    //         UPP[i][jj] = double(jj);
    //      //UPP[0][1] = (double)(1);
    //    }
    //  }  
    //
    //  XPP = new double*[ind_dim];
    //  for (int i=0; i<ind_dim; i++) {
    //    XPP[i] = new double[p];
    //      for (int j=0; j<p; j++)
    //        XPP[i][j] = double(i);//(double)rand();
    //  }
    //  YP  = new double[dep_dim];
    //  YPP = new double*[dep_dim];
    //  for (int i=0; i<dep_dim; i++)
    //      YPP[i] = new double[p];  

      // Compute adjoint using tape

    // COMM : Alternative use of functions invloving tape
    // COMM : left here for reference

      //forward(tape_tag_id,dep_dim,ind_dim,0,X,XPP,YP,YPP);
      //forward(tape_tag_id,dep_dim,ind_dim,1,X,Z);
      //std::cout << "fx1[ncell_2] = " << fx1[nb_cells/2] << std::endl;
      //std::cout << "Z[ncell_2] = " << Z[nb_cells/2] << std::endl;
      //reverse(tape_tag_id,dep_dim,ind_dim,0,p,UPP,ZPP);

      for (auto _ : state) {
         // set the left vector
        for (int j=0;j<5;j++){
          for(int i=0; i<sz; ++i) {
            dY[j*sz+i] = double(i);
          }
        }
        // Compute vector-jacobian product Z = dY^T*J
        vec_jac(tape_tag_id,dep_dim,ind_dim,0,X,dY,Z);
      }

      state.counters["lambda_rho[nb_cells/2]"] = Z[nb_cells/2];
    }
  } // reverse
} //roe_flux_case
