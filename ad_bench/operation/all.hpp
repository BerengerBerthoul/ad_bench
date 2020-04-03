#pragma once

// C++ functions
#include "ad_bench/operation/centered_gradient.hpp"
#include "ad_bench/operation/roe_flux.hpp"

// Fortran functions
extern "C" {
  void centered_gradient_  (double* dw             , double* w            , int& im, int& gh );
  void centered_gradient_d_(double* dw, double* dwd, double* w, double* wd, int& im, int& gh );
  void centered_gradient_b_(double* dw, double* dwd, double* w, double* wd, int& im, int& gh );

  void roe_flux_(
    double* rho, 
    double* velx, double* vely, double* velz, 
    double* temp, 
    double* surfx, double* surfy, double* surfz, 
    double* flux1, double* flux2, double* flux3,
    double* flux4, double* flux5,
    int& im, int& gh
  );
  void roe_flux_d_(
    double* rho, double* rhod,
    double* velx, double* velxd, double* vely, double* velyd, double* velz, double* velzd, 
    double* temp, double* tempd, 
    double* surfx, double* surfy, double* surfz, 
    double* flux1, double* flux1d, double* flux2, double* flux2d, double* flux3, double* flux3d, 
    double* flux4, double* flux4d, double* flux5, double* flux5d, 
    int& im, int& gh
  );
  void roe_flux_b_(
    double* rho, double* rhod,
    double* velx, double* velxd, double* vely, double* velyd, double* velz, double* velzd, 
    double* temp, double* tempd, 
    double* surfx, double* surfy, double* surfz, 
    double* flux1, double* flux1d, double* flux2, double* flux2d, double* flux3, double* flux3d, 
    double* flux4, double* flux4d, double* flux5, double* flux5d, 
    int& im, int& gh
  );
} // extern "C"
