#pragma once

#include <cmath>

template<typename T>
__attribute__((noinline)) // because has to be like that with the Fortran version (when we call it from C++)
                          // and we want the benchmark to be fair
void roe_flux(
  const T* __restrict__ rho,
  const T* __restrict__ velx,
  const T* __restrict__ vely,
  const T* __restrict__ velz,
  const T* __restrict__ temp,
  const double* __restrict__ surfx,
  const double* __restrict__ surfy,
  const double* __restrict__ surfz,
  T* __restrict__ flux1,
  T* __restrict__ flux2,
  T* __restrict__ flux3,
  T* __restrict__ flux4,
  T* __restrict__ flux5,
  int n_cell, int gh)
{
  constexpr double gam    = 1.4;
  constexpr double gam1   = gam-1.;
  constexpr double gam1_1 = 1./gam1;
  constexpr double rgaz   = 237.;

  // NOTE: GCC applies SIMD with the correct compilation options (-ftree-vectorize -ftree-loop-vectorize) thanks to __restrict__
  for(int i=gh; i<n_cell-gh ; ++i) {
    double sc1 = surfx[i];
    double sc2 = surfy[i];
    double sc3 = surfz[i];
    double sn  = std::sqrt(sc1*sc1 + sc2*sc2 + sc3*sc3);

    double invsn = 1./std::max(sn,1.e-32);
    double nx    = sc1*invsn;
    double ny    = sc2*invsn;
    double nz    = sc3*invsn;

    T wfl1 = rho[i-1];
    T wfr1 = rho[i  ];
    T wfl2 = velx[i-1];
    T wfr2 = velx[i  ];
    T wfl3 = vely[i-1];
    T wfr3 = vely[i  ];
    T wfl4 = velz[i-1];
    T wfr4 = velz[i  ];
    T wfl5 = temp[i-1];
    T wfr5 = temp[i  ];

    T pm = wfl1*wfl5*rgaz;
    T pp = wfr1*wfr5*rgaz;

    T hm  = gam*gam1_1*wfl5*rgaz + 0.5*(wfl2*wfl2 + wfl3*wfl3 + wfl4*wfl4);
    T hp  = gam*gam1_1*wfr5*rgaz + 0.5*(wfr2*wfr2 + wfr3*wfr3 + wfr4*wfr4);

    T fcdx1 = wfr1*wfr2 + wfl1*wfl2;
    T fcdy1 = wfr1*wfr3 + wfl1*wfl3;
    T fcdz1 = wfr1*wfr4 + wfl1*wfl4;

    T fcdx2 = wfr1*wfr2*wfr2 + pp + wfl1*wfl2*wfl2 + pm;
    T fcdy2 = wfr1*wfr2*wfr3      + wfl1*wfl2*wfl3;
    T fcdz2 = wfr1*wfr2*wfr4      + wfl1*wfl2*wfl4;

    T fcdx3 = fcdy2;
    T fcdy3 = wfr1*wfr3*wfr3 + pp + wfl1*wfl3*wfl3 + pm;
    T fcdz3 = wfr1*wfr3*wfr4      + wfl1*wfl3*wfl4;

    T fcdx4 = fcdz2;
    T fcdy4 = fcdz3;
    T fcdz4 = wfr1*wfr4*wfr4 + pp + wfl1*wfl4*wfl4 + pm;

    T fcdx5 = wfr2*wfr1*hp + wfl2*wfl1*hm;
    T fcdy5 = wfr3*wfr1*hp + wfl3*wfl1*hm;
    T fcdz5 = wfr4*wfr1*hp + wfl4*wfl1*hm;

    flux1[i] = 0.5*sn*(fcdx1*nx + fcdy1*ny + fcdz1*nz);
    flux2[i] = 0.5*sn*(fcdx2*nx + fcdy2*ny + fcdz2*nz);
    flux3[i] = 0.5*sn*(fcdx3*nx + fcdy3*ny + fcdz3*nz);
    flux4[i] = 0.5*sn*(fcdx4*nx + fcdy4*ny + fcdz4*nz);
    flux5[i] = 0.5*sn*(fcdx5*nx + fcdy5*ny + fcdz5*nz);
  }
}
