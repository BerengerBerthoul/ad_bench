#pragma once

#include <vector>
#include <iostream>
#include <chrono>

#include "ad_bench/operation/all.hpp"
#include "codi.hpp"
using codi::RealReverse;

inline
double benchmark_centered_gradient_codipack_reverse(int n_iter, int n_cell) {
  int gh = 2;
  int sz = n_cell+2*gh;
  
  // Creation
  std::vector<RealReverse>  w_vec(sz); RealReverse* w = w_vec.data();
  std::vector<RealReverse> dw_vec(sz); RealReverse* dw = dw_vec.data();
  
  // Initialize w
  for (int i=0; i<sz; ++i) {
     w[i] = i; // NOTE: unused for this particular operator
    //dw[i].setGradient(i); // 
  }
  RealReverse::TapeType& tape = codi::RealReverse::getGlobalTape();
  
  // Compute
  auto start = std::chrono::system_clock::now();
  for(int iter=0; iter < n_iter; iter++) {
    // begin recording
    tape.setActive();    
    
    // apply operator
    for(int i=0; i<sz; i++) { tape.registerInput(w[i]); } // register inputs
    centered_gradient(w, dw, n_cell, gh); // register function application
    for(int i=0; i<sz; i++) { tape.registerOutput(dw[i]); } // register outputs
    
    // stop recording
    tape.setPassive();

    // set the left vector, then evaluate
    for(int i=0; i<sz; i++) { dw[i].setGradient(i); }
    tape.evaluate();
    
    // prepare for next iteration
    if(iter != n_iter-1) tape.reset();
  }
  auto end = std::chrono::system_clock::now();
  
  std::cout << "gradient w: " << w[n_cell/2].getGradient() << "\t";
  return std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
}

inline
double benchmark_roe_flux_codipack_reverse(int n_iter, int n_cell) {
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
  codi::RealReverse::TapeType& tape = codi::RealReverse::getGlobalTape();
  
  // Compute
  auto start = std::chrono::system_clock::now();
  for (int iter=0; iter<n_iter; ++iter) {
    // begin recording
    tape.setActive();    
    
    // apply operator
    for(int i=0; i<sz; ++i) { // register inputs
      tape.registerInput(rho[i]);
      tape.registerInput(velx[i]);
      tape.registerInput(vely[i]);
      tape.registerInput(velz[i]);
      tape.registerInput(temp[i]);
    }
    roe_flux( // register function application
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
    
    // stop recording
    tape.setPassive();

    // set the left vector, then evaluate
    for(int i=0; i<sz; ++i) {
      flux1[i].setGradient(i);
      flux2[i].setGradient(i);
      flux3[i].setGradient(i);
      flux4[i].setGradient(i);
      flux5[i].setGradient(i);
    }
    tape.evaluate();
    
    if(iter != n_iter-1) tape.reset();
  }
  auto end = std::chrono::system_clock::now();
  
  std::cout << "gradient rho: " << rho[n_cell/2].getGradient() << "\t";
  return std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
}
