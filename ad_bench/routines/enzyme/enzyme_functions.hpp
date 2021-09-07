#pragma once
#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include<iostream>
#include<vector>
#include<typeinfo>
#include<cmath>

// declare external variables and functions for enzyme usage
extern "C"
{
  void __enzyme_autodiff(...) noexcept;
  int enzyme_dupnoneed;
  int enzyme_dup;
  int enzyme_const;
  int enzyme_out;
}

// begin namespace functions
namespace functions
{

// begin class CenteredGradient
template<typename T>
class CenteredGradient
{

public:

  //default constructor and destructor
  CenteredGradient();
  ~CenteredGradient();
  
  // original function
  static void f(const T *w, T *dw, int n_cell, int gh)
  {
    for(int i=gh; i<gh+n_cell; ++i) 
      dw[i] = 0.5*(w[i+1] - w[i-1]);
  }

  // differentiated function
  static void df(const T *w, T *d_w, const T* dw, T* d_dw, const int n_cell, const int gh)
  {

    __enzyme_autodiff(f, enzyme_dup, w,
      d_w, dw, d_dw, enzyme_const, n_cell, gh);

  }

};// end class CenteredGradient

// begin class RoeFlux
template<typename T>
class RoeFlux
{

public:

  //default constructor and destructor
  RoeFlux();
  ~RoeFlux();

  //original function
  static void f(const T* __restrict rho, const T* __restrict velx, const T* __restrict vely, const T* __restrict velz, const T* __restrict temp, 
  /*const T* surfx, const T* surfy, const T* surfz,*/ 
  T* flux1, T* flux2, T* flux3, T* flux4, T* flux5,
  const int n_cell, const int gh, const T* surfx, const T* surfy, const T* surfz )
  {

    constexpr double gam    = 1.4;
    constexpr double gam1   = gam-1.;
    constexpr double gam1_1 = 1./gam1;
    constexpr double rgaz   = 237.;

    for(int i=gh; i<n_cell-gh ; ++i) 
    {
      const double sc1 = surfx[i]; const double sc2 = surfy[i]; const double sc3 = surfz[i];
      const double sn  = std::sqrt(sc1*sc1 + sc2*sc2 + sc3*sc3);
    
      const double invsn = 1./std::max(sn,1.e-32);
      const double nx    = sc1*invsn; const double ny = sc2*invsn; const double nz    = sc3*invsn;
    
      const  auto wfl1 = rho[i-1];  const auto wfr1 = rho[i  ];
    
      const  auto wfl2 = velx[i-1]; const auto wfr2 = velx[i  ];
      const  auto wfl3 = vely[i-1]; const auto wfr3 = vely[i  ];
      const  auto wfl4 = velz[i-1]; const auto wfr4 = velz[i  ];
      const  auto wfl5 = temp[i-1]; const auto wfr5 = temp[i  ];
    
      const auto pm = wfl1*wfl5*rgaz; auto pp = wfr1*wfr5*rgaz;
    
      const auto hm  = gam*gam1_1*wfl5*rgaz + 0.5*(wfl2*wfl2 + wfl3*wfl3 + wfl4*wfl4);
      const auto hp  = gam*gam1_1*wfr5*rgaz + 0.5*(wfr2*wfr2 + wfr3*wfr3 + wfr4*wfr4);
    
      const auto fcdx1 = wfr1*wfr2 + wfl1*wfl2;
      const auto fcdy1 = wfr1*wfr3 + wfl1*wfl3;
      const auto fcdz1 = wfr1*wfr4 + wfl1*wfl4;
    
      const auto fcdx2 = wfr1*wfr2*wfr2 + pp + wfl1*wfl2*wfl2 + pm;
      const auto fcdy2 = wfr1*wfr2*wfr3      + wfl1*wfl2*wfl3;
      const auto fcdz2 = wfr1*wfr2*wfr4      + wfl1*wfl2*wfl4;
    
      const auto fcdx3 = fcdy2;
      const auto fcdy3 = wfr1*wfr3*wfr3 + pp + wfl1*wfl3*wfl3 + pm;
      const auto fcdz3 = wfr1*wfr3*wfr4      + wfl1*wfl3*wfl4;
    
      const auto fcdx4 = fcdz2;
      const auto fcdy4 = fcdz3;
      const auto fcdz4 = wfr1*wfr4*wfr4 + pp + wfl1*wfl4*wfl4 + pm;
    
      const auto fcdx5 = wfr2*wfr1*hp + wfl2*wfl1*hm;
      const auto fcdy5 = wfr3*wfr1*hp + wfl3*wfl1*hm;
      const auto fcdz5 = wfr4*wfr1*hp + wfl4*wfl1*hm;
   

      flux1[i] = 0.5*sn*(fcdx1*nx + fcdy1*ny + fcdz1*nz);
      flux2[i] = 0.5*sn*(fcdx2*nx + fcdy2*ny + fcdz2*nz);
      flux3[i] = 0.5*sn*(fcdx3*nx + fcdy3*ny + fcdz3*nz);
      flux4[i] = 0.5*sn*(fcdx4*nx + fcdy4*ny + fcdz4*nz);
      flux5[i] = 0.5*sn*(fcdx5*nx + fcdy5*ny + fcdz5*nz);
    }

  }

  // differentiated function
  static void df(const T *rho, T *rho_b, const T* velx, T* velx_b, const T* vely, T* vely_b,
    const T* velz, T* velz_b, const T* temp, T* temp_b, 
    /**const T * surfx, const T * surfx_b, 
    const T * surfy, const T * surfy_b, const T * surfz, const T * surfz_b,**/
    const T* flux1, T* flux1_b, const T* flux2, T* flux2_b, const T* flux3, T* flux3_b, 
    const T* flux4, T* flux4_b, const T* flux5, T* flux5_b, const int n_cell, const int gh,
    const T* surfx, const T* surfy, const T* surfz )
  {

    __enzyme_autodiff(f, 
       enzyme_dup, rho, 
       rho_b, 
       enzyme_dup, velx, 
       velx_b, 
       enzyme_dup, vely, 
       vely_b, 
       enzyme_dup, velz, 
       velz_b,
       enzyme_dup, temp, 
       temp_b, 
       //enzyme_const, surfx, /**surfx_b,**/ 
       //enzyme_const, surfy, /**surfy_b,**/ 
       //enzyme_const, surfz, /**surfz_b,**/ 
       enzyme_dupnoneed, flux1, 
       flux1_b,
       enzyme_dupnoneed, flux2, 
       flux2_b, 
       enzyme_dupnoneed, flux3, 
       flux3_b, 
       enzyme_dupnoneed, flux4, 
       flux4_b, 
       enzyme_dupnoneed, flux5, 
       flux5_b, 
       enzyme_const, n_cell, enzyme_const, gh, 
       enzyme_const, surfx, enzyme_const, surfy, enzyme_const, surfz);
  }

}; // end class RoeFlux


}// end namespace functions

#endif
