#pragma once

#include <cmath>

template<typename T>
void roe_flux(
  const T* rho, 
  const T* velx, 
  const T* vely, 
  const T* velz, 
  const T* temp, 
  const double* surfx, 
  const double* surfy, 
  const double* surfz, 
  T* flux1, 
  T* flux2, 
  T* flux3, 
  T* flux4, 
  T* flux5, 
  int n_cell, int gh
) {
  constexpr double gam    = 1.4;
  constexpr double gam1   = gam-1.;
  constexpr double gam1_1 = 1./gam1;
  constexpr double rgaz   = 237.;
  
  // #pragma omp simd TODO: actually makes it slower
  for(int i=gh; i<n_cell-gh ; ++i) {
    auto sc1 = surfx[i];
    auto sc2 = surfy[i];
    auto sc3 = surfz[i];
    auto sn  = std::sqrt(sc1*sc1 + sc2*sc2 + sc3*sc3);
    
    auto invsn = 1./std::max(sn,1.e-32);
    auto nx    = sc1*invsn;
    auto ny    = sc2*invsn;
    auto nz    = sc3*invsn;
    
    auto wfl1 = rho[i-1];
    auto wfr1 = rho[i  ];
    
    auto wfl2 = velx[i-1];
    auto wfr2 = velx[i  ];
    auto wfl3 = vely[i-1];
    auto wfr3 = vely[i  ];
    auto wfl4 = velz[i-1];
    auto wfr4 = velz[i  ];
    auto wfl5 = temp[i-1];
    auto wfr5 = temp[i  ];
    
    auto pm = wfl1*wfl5*rgaz;
    auto pp = wfr1*wfr5*rgaz;
    
    auto hm  = gam*gam1_1*wfl5*rgaz + 0.5*(wfl2*wfl2 + wfl3*wfl3 + wfl4*wfl4);
    auto hp  = gam*gam1_1*wfr5*rgaz + 0.5*(wfr2*wfr2 + wfr3*wfr3 + wfr4*wfr4);
    
    auto fcdx1 = wfr1*wfr2 + wfl1*wfl2;
    auto fcdy1 = wfr1*wfr3 + wfl1*wfl3;
    auto fcdz1 = wfr1*wfr4 + wfl1*wfl4;

    auto fcdx2 = wfr1*wfr2*wfr2 + pp + wfl1*wfl2*wfl2 + pm;
    auto fcdy2 = wfr1*wfr2*wfr3      + wfl1*wfl2*wfl3;
    auto fcdz2 = wfr1*wfr2*wfr4      + wfl1*wfl2*wfl4;

    auto fcdx3 = fcdy2;
    auto fcdy3 = wfr1*wfr3*wfr3 + pp + wfl1*wfl3*wfl3 + pm;
    auto fcdz3 = wfr1*wfr3*wfr4      + wfl1*wfl3*wfl4;

    auto fcdx4 = fcdz2;
    auto fcdy4 = fcdz3;
    auto fcdz4 = wfr1*wfr4*wfr4 + pp + wfl1*wfl4*wfl4 + pm;

    auto fcdx5 = wfr2*wfr1*hp + wfl2*wfl1*hm;
    auto fcdy5 = wfr3*wfr1*hp + wfl3*wfl1*hm;
    auto fcdz5 = wfr4*wfr1*hp + wfl4*wfl1*hm;

    flux1[i] = 0.5*sn*(fcdx1*nx + fcdy1*ny + fcdz1*nz);
    flux2[i] = 0.5*sn*(fcdx2*nx + fcdy2*ny + fcdz2*nz);
    flux3[i] = 0.5*sn*(fcdx3*nx + fcdy3*ny + fcdz3*nz);
    flux4[i] = 0.5*sn*(fcdx4*nx + fcdy4*ny + fcdz4*nz);
    flux5[i] = 0.5*sn*(fcdx5*nx + fcdy5*ny + fcdz5*nz);
  }
}
