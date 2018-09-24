

/* ------------------------------------------------------ */
template<typename T>
T func(const T& x) {
  if(x>0)
      return x*x;
  else
    return min(x,1);
}

/* ------------------------------------------------------ */
template<typename T>
T stencilO2(const std::vector<T>& w, std::vector<T>& dw, int gh)
{
  for(int icell=gh; icell < w.size() -gh ; icell++)
  {
     dw[icell] = 0.5*(w[icell+1] - w[icell-1]);
  }
}

/* ------------------------------------------------------ */
template<typename T>
T fluxRoe(const std::vector<T>& rho, 
          const std::vector<T>& velx, 
          const std::vector<T>& vely, 
          const std::vector<T>& velz, 
          const std::vector<T>& temp, 
          const std::vector<double>& surfx, 
          const std::vector<double>& surfy, 
          const std::vector<double>& surfz, 
          std::vector<T>& flux1, 
          std::vector<T>& flux2, 
          std::vector<T>& flux3, 
          std::vector<T>& flux4, 
          std::vector<T>& flux5, 
          int gh) {
  
  constexpr double gam    = 1.4;
  constexpr double gam1   = gam-1.;
  constexpr double gam1_1 = 1./gam1;
  constexpr double rgaz   = 237.;
  
  for(int icell=gh; icell < rho.size() -gh ; icell++)
  {
    auto sc1 = surfx[icell];
    auto sc2 = surfy[icell];
    auto sc3 = surfz[icell];
    auto sn  = std::sqrt(sc1*sc1 + sc2*sc2 + sc3*sc3);
    
    auto invsn = 1./std::max(sn,1.e-32);
    auto nx    = sc1*invsn;
    auto ny    = sc2*invsn;
    auto nz    = sc3*invsn;
    
    auto wfl1 = rho[icell-1];
    auto wfr1 = rho[icell  ];
    
    auto wfl2 = velx[icell-1];
    auto wfr2 = velx[icell  ];
    auto wfl3 = vely[icell-1];
    auto wfr3 = vely[icell  ];
    auto wfl4 = velz[icell-1];
    auto wfr4 = velz[icell  ];
    auto wfl5 = temp[icell-1];
    auto wfr5 = temp[icell  ];
    
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

    flux1[icell] = 0.5*sn*(fcdx1*nx + fcdy1*ny + fcdz1*nz);
    flux2[icell] = 0.5*sn*(fcdx2*nx + fcdy2*ny + fcdz2*nz);
    flux3[icell] = 0.5*sn*(fcdx3*nx + fcdy3*ny + fcdz3*nz);
    flux4[icell] = 0.5*sn*(fcdx4*nx + fcdy4*ny + fcdz4*nz);
    flux5[icell] = 0.5*sn*(fcdx5*nx + fcdy5*ny + fcdz5*nz);
    
    
  }
  
}
