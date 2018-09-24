!        Generated by TAPENADE     (INRIA, Ecuador team)
!  Tapenade 3.13 (r6666M) - 28 May 2018 09:28
!
!  Differentiation of fluxroe in forward (tangent) mode:
!   variations   of useful results: flux1 flux2 flux3 flux4 flux5
!   with respect to varying inputs: temp velx vely velz rho
!   RW status of diff variables: temp:in velx:in vely:in velz:in
!                flux1:out flux2:out flux3:out flux4:out flux5:out
!                rho:in
SUBROUTINE FLUXROE_D(rho, rhod, velx, velxd, vely, velyd, velz, velzd, &
& temp, tempd, surfx, surfy, surfz, flux1, flux1d, flux2, flux2d, flux3&
& , flux3d, flux4, flux4d, flux5, flux5d, im, gh)
  IMPLICIT NONE
! -------------------------------------------------
  INTEGER, INTENT(IN) :: im, gh
  REAL*8, DIMENSION(1-gh:im+gh), INTENT(INOUT) :: rho
  REAL*8, DIMENSION(1-gh:im+gh), INTENT(INOUT) :: rhod
  REAL*8, DIMENSION(1-gh:im+gh), INTENT(INOUT) :: velx, vely, velz
  REAL*8, DIMENSION(1-gh:im+gh), INTENT(INOUT) :: velxd, velyd, velzd
  REAL*8, DIMENSION(1-gh:im+gh), INTENT(INOUT) :: temp
  REAL*8, DIMENSION(1-gh:im+gh), INTENT(INOUT) :: tempd
  REAL*8, DIMENSION(1-gh:im+gh), INTENT(INOUT) :: surfx
  REAL*8, DIMENSION(1-gh:im+gh), INTENT(INOUT) :: surfy
  REAL*8, DIMENSION(1-gh:im+gh), INTENT(INOUT) :: surfz
  REAL*8, DIMENSION(1-gh:im+gh), INTENT(INOUT) :: flux1
  REAL*8, DIMENSION(1-gh:im+gh), INTENT(INOUT) :: flux1d
  REAL*8, DIMENSION(1-gh:im+gh), INTENT(INOUT) :: flux2
  REAL*8, DIMENSION(1-gh:im+gh), INTENT(INOUT) :: flux2d
  REAL*8, DIMENSION(1-gh:im+gh), INTENT(INOUT) :: flux3
  REAL*8, DIMENSION(1-gh:im+gh), INTENT(INOUT) :: flux3d
  REAL*8, DIMENSION(1-gh:im+gh), INTENT(INOUT) :: flux4
  REAL*8, DIMENSION(1-gh:im+gh), INTENT(INOUT) :: flux4d
  REAL*8, DIMENSION(1-gh:im+gh), INTENT(INOUT) :: flux5
  REAL*8, DIMENSION(1-gh:im+gh), INTENT(INOUT) :: flux5d
  INTEGER :: i
  REAL*8 :: sc1, sc2, sc3
  REAL*8 :: gam
  REAL*8 :: rgaz
  REAL*8 :: wfl1, wfl2, wfl3, wfl4, wfl5
  REAL*8 :: wfl1d, wfl2d, wfl3d, wfl4d, wfl5d
  REAL*8 :: wfr1, wfr2, wfr3, wfr4, wfr5
  REAL*8 :: wfr1d, wfr2d, wfr3d, wfr4d, wfr5d
  REAL*8 :: gam1, gam1_1
  REAL*8 :: invsn
  REAL*8 :: rp, up, vp, wp, pp, rm, um, vm, wm, pm
  REAL*8 :: ppd, pmd
! to work with (roe mean)
  REAL*8 :: r, hm, hp
  REAL*8 :: hmd, hpd
! aerodynamic local values
  REAL*8 :: uu, vv, ww, ee, rr, hh, cc
! to work with (d1 computation)
  REAL*8 :: du, dv, dw, dvn, dd1, dd4, dd5
! idem
  REAL*8 :: df11, df12, df13, df14, df15
! idem
  REAL*8 :: df451, df452, df453, df454, df455
  REAL*8 :: nx, ny, nz, sn, vn
! small = pctrad% of spectral radius
  REAL*8 :: small
  REAL*8 :: aa1, aa4, aa5
  REAL*8 :: fcdx1, fcdx2, fcdx3, fcdx4, fcdx5, fcdx6
  REAL*8 :: fcdx1d, fcdx2d, fcdx3d, fcdx4d, fcdx5d
! flux densities
  REAL*8 :: fcdy1, fcdy2, fcdy3, fcdy4, fcdy5, fcdy6
  REAL*8 :: fcdy1d, fcdy2d, fcdy3d, fcdy4d, fcdy5d
  REAL*8 :: fcdz1, fcdz2, fcdz3, fcdz4, fcdz5, fcdz6
  REAL*8 :: fcdz1d, fcdz2d, fcdz3d, fcdz4d, fcdz5d
  REAL*8 :: flc1, flc2, flc3, flc4, flc5, flc6
  REAL*8 :: flc1d, flc2d, flc3d, flc4d, flc5d
  REAL*8 :: fld1, fld2, fld3, fld4, fld5, fld6
  INTRINSIC SQRT
  INTRINSIC MAX
  REAL*8 :: max1
  REAL*8 :: arg1
! -------------------------------------------------
  gam = 1.4
  gam1 = gam - 1.
  gam1_1 = 1./gam1
  rgaz = 237.
  flux1d = 0.0_8
  flux2d = 0.0_8
  flux3d = 0.0_8
  flux4d = 0.0_8
  flux5d = 0.0_8
!$DIR SIMD
  DO i=1,im
    sc1 = surfx(i)
    sc2 = surfy(i)
    sc3 = surfz(i)
    arg1 = sc1*sc1 + sc2*sc2 + sc3*sc3
    sn = SQRT(arg1)
    IF (sn .LT. 1.d-32) THEN
      max1 = 1.d-32
    ELSE
      max1 = sn
    END IF
    invsn = 1.d0/max1
    nx = sc1*invsn
    ny = sc2*invsn
    nz = sc3*invsn
    wfl1d = rhod(i-1)
    wfl1 = rho(i-1)
    wfr1d = rhod(i)
    wfr1 = rho(i)
    wfl2d = velxd(i-1)
    wfl2 = velx(i-1)
    wfr2d = velxd(i)
    wfr2 = velx(i)
    wfl3d = velyd(i-1)
    wfl3 = vely(i-1)
    wfr3d = velyd(i)
    wfr3 = vely(i)
    wfl4d = velzd(i-1)
    wfl4 = velz(i-1)
    wfr4d = velzd(i)
    wfr4 = velz(i)
    wfl5d = tempd(i-1)
    wfl5 = temp(i-1)
    wfr5d = tempd(i)
    wfr5 = temp(i)
    pmd = rgaz*(wfl1d*wfl5+wfl1*wfl5d)
    pm = wfl1*wfl5*rgaz
    ppd = rgaz*(wfr1d*wfr5+wfr1*wfr5d)
    pp = wfr1*wfr5*rgaz
    hmd = gam*gam1_1*rgaz*wfl5d + 0.5d0*(wfl2d*wfl2+wfl2*wfl2d+wfl3d*&
&     wfl3+wfl3*wfl3d+wfl4d*wfl4+wfl4*wfl4d)
    hm = gam*gam1_1*wfl5*rgaz + 0.5d0*(wfl2*wfl2+wfl3*wfl3+wfl4*wfl4)
    hpd = gam*gam1_1*rgaz*wfr5d + 0.5d0*(wfr2d*wfr2+wfr2*wfr2d+wfr3d*&
&     wfr3+wfr3*wfr3d+wfr4d*wfr4+wfr4*wfr4d)
    hp = gam*gam1_1*wfr5*rgaz + 0.5d0*(wfr2*wfr2+wfr3*wfr3+wfr4*wfr4)
    fcdx1d = wfr1d*wfr2 + wfr1*wfr2d + wfl1d*wfl2 + wfl1*wfl2d
    fcdx1 = wfr1*wfr2 + wfl1*wfl2
    fcdy1d = wfr1d*wfr3 + wfr1*wfr3d + wfl1d*wfl3 + wfl1*wfl3d
    fcdy1 = wfr1*wfr3 + wfl1*wfl3
    fcdz1d = wfr1d*wfr4 + wfr1*wfr4d + wfl1d*wfl4 + wfl1*wfl4d
    fcdz1 = wfr1*wfr4 + wfl1*wfl4
    fcdx2d = (wfr1d*wfr2+wfr1*wfr2d)*wfr2 + wfr1*wfr2*wfr2d + ppd + (&
&     wfl1d*wfl2+wfl1*wfl2d)*wfl2 + wfl1*wfl2*wfl2d + pmd
    fcdx2 = wfr1*wfr2*wfr2 + pp + wfl1*wfl2*wfl2 + pm
    fcdy2d = (wfr1d*wfr2+wfr1*wfr2d)*wfr3 + wfr1*wfr2*wfr3d + (wfl1d*&
&     wfl2+wfl1*wfl2d)*wfl3 + wfl1*wfl2*wfl3d
    fcdy2 = wfr1*wfr2*wfr3 + wfl1*wfl2*wfl3
    fcdz2d = (wfr1d*wfr2+wfr1*wfr2d)*wfr4 + wfr1*wfr2*wfr4d + (wfl1d*&
&     wfl2+wfl1*wfl2d)*wfl4 + wfl1*wfl2*wfl4d
    fcdz2 = wfr1*wfr2*wfr4 + wfl1*wfl2*wfl4
    fcdx3d = fcdy2d
    fcdx3 = fcdy2
    fcdy3d = (wfr1d*wfr3+wfr1*wfr3d)*wfr3 + wfr1*wfr3*wfr3d + ppd + (&
&     wfl1d*wfl3+wfl1*wfl3d)*wfl3 + wfl1*wfl3*wfl3d + pmd
    fcdy3 = wfr1*wfr3*wfr3 + pp + wfl1*wfl3*wfl3 + pm
    fcdz3d = (wfr1d*wfr3+wfr1*wfr3d)*wfr4 + wfr1*wfr3*wfr4d + (wfl1d*&
&     wfl3+wfl1*wfl3d)*wfl4 + wfl1*wfl3*wfl4d
    fcdz3 = wfr1*wfr3*wfr4 + wfl1*wfl3*wfl4
    fcdx4d = fcdz2d
    fcdx4 = fcdz2
    fcdy4d = fcdz3d
    fcdy4 = fcdz3
    fcdz4d = (wfr1d*wfr4+wfr1*wfr4d)*wfr4 + wfr1*wfr4*wfr4d + ppd + (&
&     wfl1d*wfl4+wfl1*wfl4d)*wfl4 + wfl1*wfl4*wfl4d + pmd
    fcdz4 = wfr1*wfr4*wfr4 + pp + wfl1*wfl4*wfl4 + pm
    fcdx5d = (wfr2d*wfr1+wfr2*wfr1d)*hp + wfr2*wfr1*hpd + (wfl2d*wfl1+&
&     wfl2*wfl1d)*hm + wfl2*wfl1*hmd
    fcdx5 = wfr2*wfr1*hp + wfl2*wfl1*hm
    fcdy5d = (wfr3d*wfr1+wfr3*wfr1d)*hp + wfr3*wfr1*hpd + (wfl3d*wfl1+&
&     wfl3*wfl1d)*hm + wfl3*wfl1*hmd
    fcdy5 = wfr3*wfr1*hp + wfl3*wfl1*hm
    fcdz5d = (wfr4d*wfr1+wfr4*wfr1d)*hp + wfr4*wfr1*hpd + (wfl4d*wfl1+&
&     wfl4*wfl1d)*hm + wfl4*wfl1*hmd
    fcdz5 = wfr4*wfr1*hp + wfl4*wfl1*hm
    flc1d = 0.5d0*sn*(nx*fcdx1d+ny*fcdy1d+nz*fcdz1d)
    flc1 = 0.5d0*sn*(fcdx1*nx+fcdy1*ny+fcdz1*nz-df11-df451)
    flc2d = 0.5d0*sn*(nx*fcdx2d+ny*fcdy2d+nz*fcdz2d)
    flc2 = 0.5d0*sn*(fcdx2*nx+fcdy2*ny+fcdz2*nz-df12-df452)
    flc3d = 0.5d0*sn*(nx*fcdx3d+ny*fcdy3d+nz*fcdz3d)
    flc3 = 0.5d0*sn*(fcdx3*nx+fcdy3*ny+fcdz3*nz-df13-df453)
    flc4d = 0.5d0*sn*(nx*fcdx4d+ny*fcdy4d+nz*fcdz4d)
    flc4 = 0.5d0*sn*(fcdx4*nx+fcdy4*ny+fcdz4*nz-df14-df454)
    flc5d = 0.5d0*sn*(nx*fcdx5d+ny*fcdy5d+nz*fcdz5d)
    flc5 = 0.5d0*sn*(fcdx5*nx+fcdy5*ny+fcdz5*nz-df15-df455)
    flux1d(i) = flc1d
    flux1(i) = flc1
    flux2d(i) = flc2d
    flux2(i) = flc2
    flux3d(i) = flc3d
    flux3(i) = flc3
    flux4d(i) = flc4d
    flux4(i) = flc4
    flux5d(i) = flc5d
    flux5(i) = flc5
  END DO
END SUBROUTINE FLUXROE_D
