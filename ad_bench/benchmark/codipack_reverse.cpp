#include "ad_bench/benchmark/all.hpp"

#include <vector>
#include <iostream>
#include <chrono>

#include "ad_bench/operation/all.hpp"
#include "codi.hpp"
using codi::RealReverse;

std::pair<double,double> benchmark_centered_gradient_codipack_reverse(int n_iter, int n_cell) {
  int gh = 2;
  int sz = n_cell+2*gh;
  
  // Creation
  std::vector<RealReverse>  w_vec(sz); RealReverse* w = w_vec.data();
  std::vector<RealReverse> dw_vec(sz); RealReverse* dw = dw_vec.data();
  
  // Initialize w
  for (int i=0; i<sz; ++i) {
     w[i] = i; // NOTE: unused for this particular operator
  }

  // Configure tape
  RealReverse::TapeType& tape = codi::RealReverse::getGlobalTape();

  // 1. begin recording
  tape.setActive();    

  // 2. apply operator
  for(int i=0; i<sz; i++) { tape.registerInput(w[i]); } // register inputs
  centered_gradient(w, dw, n_cell, gh); // register function evaluation
  for(int i=0; i<sz; i++) { tape.registerOutput(dw[i]); } // register outputs
  
  // 3. stop recording
  tape.setPassive();

  for(int i=0; i<sz; i++) { dw[i].setGradient(i); }
  
  // Compute
  auto start = std::chrono::system_clock::now();
  for(int iter=0; iter < n_iter; ++iter) {
    // set the left vector
    for(int i=0; i<sz; ++i) {
      dw[i].setGradient(double(i)/(iter+1));
    }

    // evaluate the tape i.e. execute adjoint code
    tape.evaluate();
  }
  auto end = std::chrono::system_clock::now();
  
  return std::make_pair(
    w[n_cell/2].getGradient(),
    std::chrono::duration<double>(end-start).count()
  );
}

std::pair<double,double> benchmark_roe_flux_codipack_reverse(int n_iter, int n_cell) {
  int gh = 2;
  int sz = n_cell+2*gh;
  
  // Creation
  std::vector<double>      surfx_vec(sz); double*      surfx = surfx_vec.data();
  std::vector<double>      surfy_vec(sz); double*      surfy = surfy_vec.data();
  std::vector<double>      surfz_vec(sz); double*      surfz = surfz_vec.data();

  std::vector<RealReverse>   rho_vec(sz); RealReverse*   rho =   rho_vec.data();
  std::vector<RealReverse>  velx_vec(sz); RealReverse*  velx =  velx_vec.data();
  std::vector<RealReverse>  vely_vec(sz); RealReverse*  vely =  vely_vec.data();
  std::vector<RealReverse>  velz_vec(sz); RealReverse*  velz =  velz_vec.data();
  std::vector<RealReverse>  temp_vec(sz); RealReverse*  temp =  temp_vec.data();
  std::vector<RealReverse> flux1_vec(sz); RealReverse* flux1 = flux1_vec.data();
  std::vector<RealReverse> flux2_vec(sz); RealReverse* flux2 = flux2_vec.data();
  std::vector<RealReverse> flux3_vec(sz); RealReverse* flux3 = flux3_vec.data();
  std::vector<RealReverse> flux4_vec(sz); RealReverse* flux4 = flux4_vec.data();
  std::vector<RealReverse> flux5_vec(sz); RealReverse* flux5 = flux5_vec.data();
  
  // Initialize
  for(int i=0; i<sz; ++i){
    surfx[i] = i; 
    surfy[i] = i; 
    surfz[i] = i; 

    rho  [i] = i; 
    velx [i] = i; 
    vely [i] = i; 
    velz [i] = i; 
    temp [i] = i; 
  }

  // Configure tape
  RealReverse::TapeType& tape = codi::RealReverse::getGlobalTape();

  // 1. begin recording
  tape.setActive();    

  // 2. apply operator
  for(int i=0; i<sz; ++i) { // register inputs
    tape.registerInput(rho[i]);
    tape.registerInput(velx[i]);
    tape.registerInput(vely[i]);
    tape.registerInput(velz[i]);
    tape.registerInput(temp[i]);
  }
  roe_flux( // register function evaluation
    rho, velx, vely, velz, temp, 
    surfx, surfy, surfz, 
    flux1, flux2, flux3, flux4, flux5, 
    n_cell, gh
  );
  for(int i=0; i<sz; ++i) { // register outputs
    tape.registerOutput(flux1[i]);
    tape.registerOutput(flux2[i]);
    tape.registerOutput(flux3[i]);
    tape.registerOutput(flux4[i]);
    tape.registerOutput(flux5[i]);
  }
  
  // 3. stop recording
  tape.setPassive();

  for(int i=0; i<sz; ++i) {
  }
  
  // Compute
  auto start = std::chrono::system_clock::now();
  for (int iter=0; iter<n_iter; ++iter) {
    // set the left vector
    for(int i=0; i<sz; ++i) {
      flux1[i].setGradient(double(i)/(iter+1));
      flux2[i].setGradient(double(i)/(iter+1));
      flux3[i].setGradient(double(i)/(iter+1));
      flux4[i].setGradient(double(i)/(iter+1));
      flux5[i].setGradient(double(i)/(iter+1));
    }
    // evaluate the tape i.e. execute adjoint code
    tape.evaluate();
  }
  auto end = std::chrono::system_clock::now();
  
  return std::make_pair(
    rho[n_cell/2].getGradient(),
    std::chrono::duration<double>(end-start).count()
  );
}
