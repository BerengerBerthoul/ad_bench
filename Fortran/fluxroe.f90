subroutine fluxroe(rho,                 &
                   velx, vely, velz,    &
                   temp,                &
                   surfx,               &
                   surfy,               &
                   surfz,               &
                   flux1,               &
                   flux2,               &
                   flux3,               &
                   flux4,               &
                   flux5,               &
                   im, gh)
  implicit none
  ! -------------------------------------------------
  integer,intent(in) :: im, gh
  real(8),dimension(1-gh:im+gh),intent(inout) :: rho
  real(8),dimension(1-gh:im+gh),intent(inout) :: velx, vely, velz
  real(8),dimension(1-gh:im+gh),intent(inout) :: temp
  real(8),dimension(1-gh:im+gh),intent(inout) :: surfx
  real(8),dimension(1-gh:im+gh),intent(inout) :: surfy
  real(8),dimension(1-gh:im+gh),intent(inout) :: surfz
  real(8),dimension(1-gh:im+gh),intent(inout) :: flux1
  real(8),dimension(1-gh:im+gh),intent(inout) :: flux2
  real(8),dimension(1-gh:im+gh),intent(inout) :: flux3
  real(8),dimension(1-gh:im+gh),intent(inout) :: flux4
  real(8),dimension(1-gh:im+gh),intent(inout) :: flux5
  integer :: i
  real(8) :: sc1, sc2, sc3
  real(8) :: gam
  real(8) :: rgaz
  real(8) :: wfl1, wfl2, wfl3, wfl4, wfl5
  real(8) :: wfr1, wfr2, wfr3, wfr4, wfr5
  real(8) :: gam1, gam1_1
  real(8) :: invsn
  real(8) :: rp,up,vp,wp,pp,rm,um,vm,wm,pm
  real(8) :: r,hm,hp                                 ! to work with (roe mean)
  real(8) :: uu,vv,ww,ee,rr,hh,cc                    ! aerodynamic local values
  real(8) :: du,dv,dw,dvn,dd1,dd4,dd5                ! to work with (d1 computation)
  real(8) :: df11,df12,df13,df14,df15                ! idem
  real(8) :: df451,df452,df453,df454,df455           ! idem
  real(8) :: nx,ny,nz,sn,vn
  real(8) :: small                                   ! small = pctrad% of spectral radius
  
  real(8) :: aa1,aa4,aa5
  real(8) :: fcdx1,fcdx2,fcdx3,fcdx4,fcdx5,fcdx6
  real(8) :: fcdy1,fcdy2,fcdy3,fcdy4,fcdy5,fcdy6           ! flux densities
  real(8) :: fcdz1,fcdz2,fcdz3,fcdz4,fcdz5,fcdz6
  real(8) :: flc1,flc2,flc3,flc4,flc5,flc6
  real(8) :: fld1,fld2,fld3,fld4,fld5,fld6
  ! -------------------------------------------------
  gam  = 1.4
  gam1   = gam-1.
  gam1_1 = 1./gam1
  rgaz   = 237.
  
  
  !$DIR SIMD
  do i=1,im
    
    sc1 = surfx(i)
    sc2 = surfy(i)
    sc3 = surfz(i)
    sn  = sqrt(sc1*sc1 + sc2*sc2 + sc3*sc3)
    invsn = 1.d0/MAX(sn,1.d-32)
    
    nx    = sc1*invsn
    ny    = sc2*invsn
    nz    = sc3*invsn


    wfl1 = rho(i-1)
    wfr1 = rho(i)
    
    wfl2 = velx(i-1)
    wfr2 = velx(i  )
    
    wfl3 = vely(i-1)
    wfr3 = vely(i  )
    
    wfl4 = velz(i-1)
    wfr4 = velz(i  )
    
    wfl5 = temp(i-1)
    wfr5 = temp(i)
    pm = wfl1*wfl5*rgaz
    pp = wfr1*wfr5*rgaz
    
    hm  = gam*gam1_1*wfl5*rgaz + 0.5d0*(wfl2*wfl2 + wfl3*wfl3 + wfl4*wfl4 )
    hp  = gam*gam1_1*wfr5*rgaz + 0.5d0*(wfr2*wfr2 + wfr3*wfr3 + wfr4*wfr4 )

    fcdx1 = wfr1*wfr2 + wfl1*wfl2
    fcdy1 = wfr1*wfr3 + wfl1*wfl3
    fcdz1 = wfr1*wfr4 + wfl1*wfl4

    fcdx2 = wfr1*wfr2*wfr2 + pp + wfl1*wfl2*wfl2 + pm
    fcdy2 = wfr1*wfr2*wfr3      + wfl1*wfl2*wfl3
    fcdz2 = wfr1*wfr2*wfr4      + wfl1*wfl2*wfl4

    fcdx3 = fcdy2
    fcdy3 = wfr1*wfr3*wfr3 + pp + wfl1*wfl3*wfl3 + pm
    fcdz3 = wfr1*wfr3*wfr4      + wfl1*wfl3*wfl4

    fcdx4 = fcdz2
    fcdy4 = fcdz3
    fcdz4 = wfr1*wfr4*wfr4 + pp + wfl1*wfl4*wfl4 + pm

    fcdx5 = wfr2*wfr1*hp + wfl2*wfl1*hm
    fcdy5 = wfr3*wfr1*hp + wfl3*wfl1*hm
    fcdz5 = wfr4*wfr1*hp + wfl4*wfl1*hm

    flc1 = 0.5d0*sn*(fcdx1*nx + fcdy1*ny + fcdz1*nz - df11 - df451)
    flc2 = 0.5d0*sn*(fcdx2*nx + fcdy2*ny + fcdz2*nz - df12 - df452)
    flc3 = 0.5d0*sn*(fcdx3*nx + fcdy3*ny + fcdz3*nz - df13 - df453)
    flc4 = 0.5d0*sn*(fcdx4*nx + fcdy4*ny + fcdz4*nz - df14 - df454)
    flc5 = 0.5d0*sn*(fcdx5*nx + fcdy5*ny + fcdz5*nz - df15 - df455)

    flux1(i) = flc1
    flux2(i) = flc2
    flux3(i) = flc3
    flux4(i) = flc4
    flux5(i) = flc5
  end do
end subroutine
