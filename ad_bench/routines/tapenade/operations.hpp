#pragma once

// Fortran functions
extern "C"
{
  void centered_gradient_b_(double* w, double* wd, double* dw, double* dwd, int& im, int& gh );

  void roe_flux_b_(
    double* rho, double* rhod,
    double* velx, double* velxd, double* vely, double* velyd, double* velz, double* velzd,
    double* temp, double* tempd,
    double* surfx, double* surfy, double* surfz,
    double* flux1, double* flux1d, double* flux2, double* flux2d, double* flux3, double* flux3d,
    double* flux4, double* flux4d, double* flux5, double* flux5d,
    int& im, int& gh
  );
}//end extern "C"
