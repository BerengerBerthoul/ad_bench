!        Generated by TAPENADE     (INRIA, Ecuador team)
!  Tapenade 3.13 (r6666M) - 28 May 2018 09:28
!
!  Differentiation of fluxroe in forward (tangent) mode (with options with!SliceDeadControl with!SliceDeadInstrs):
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
    ppd = rgaz*(wfr1d*wfr5+wfr1*wfr5d)
    hmd = gam*gam1_1*rgaz*wfl5d + 0.5d0*(wfl2d*wfl2+wfl2*wfl2d+wfl3d*&
&     wfl3+wfl3*wfl3d+wfl4d*wfl4+wfl4*wfl4d)
    hm = gam*gam1_1*wfl5*rgaz + 0.5d0*(wfl2*wfl2+wfl3*wfl3+wfl4*wfl4)
    hpd = gam*gam1_1*rgaz*wfr5d + 0.5d0*(wfr2d*wfr2+wfr2*wfr2d+wfr3d*&
&     wfr3+wfr3*wfr3d+wfr4d*wfr4+wfr4*wfr4d)
    hp = gam*gam1_1*wfr5*rgaz + 0.5d0*(wfr2*wfr2+wfr3*wfr3+wfr4*wfr4)
    fcdx1d = wfr1d*wfr2 + wfr1*wfr2d + wfl1d*wfl2 + wfl1*wfl2d
    fcdy1d = wfr1d*wfr3 + wfr1*wfr3d + wfl1d*wfl3 + wfl1*wfl3d
    fcdz1d = wfr1d*wfr4 + wfr1*wfr4d + wfl1d*wfl4 + wfl1*wfl4d
    fcdx2d = (wfr1d*wfr2+wfr1*wfr2d)*wfr2 + wfr1*wfr2*wfr2d + ppd + (&
&     wfl1d*wfl2+wfl1*wfl2d)*wfl2 + wfl1*wfl2*wfl2d + pmd
    fcdy2d = (wfr1d*wfr2+wfr1*wfr2d)*wfr3 + wfr1*wfr2*wfr3d + (wfl1d*&
&     wfl2+wfl1*wfl2d)*wfl3 + wfl1*wfl2*wfl3d
    fcdz2d = (wfr1d*wfr2+wfr1*wfr2d)*wfr4 + wfr1*wfr2*wfr4d + (wfl1d*&
&     wfl2+wfl1*wfl2d)*wfl4 + wfl1*wfl2*wfl4d
    fcdx3d = fcdy2d
    fcdy3d = (wfr1d*wfr3+wfr1*wfr3d)*wfr3 + wfr1*wfr3*wfr3d + ppd + (&
&     wfl1d*wfl3+wfl1*wfl3d)*wfl3 + wfl1*wfl3*wfl3d + pmd
    fcdz3d = (wfr1d*wfr3+wfr1*wfr3d)*wfr4 + wfr1*wfr3*wfr4d + (wfl1d*&
&     wfl3+wfl1*wfl3d)*wfl4 + wfl1*wfl3*wfl4d
    fcdx4d = fcdz2d
    fcdy4d = fcdz3d
    fcdz4d = (wfr1d*wfr4+wfr1*wfr4d)*wfr4 + wfr1*wfr4*wfr4d + ppd + (&
&     wfl1d*wfl4+wfl1*wfl4d)*wfl4 + wfl1*wfl4*wfl4d + pmd
    fcdx5d = (wfr2d*wfr1+wfr2*wfr1d)*hp + wfr2*wfr1*hpd + (wfl2d*wfl1+&
&     wfl2*wfl1d)*hm + wfl2*wfl1*hmd
    fcdy5d = (wfr3d*wfr1+wfr3*wfr1d)*hp + wfr3*wfr1*hpd + (wfl3d*wfl1+&
&     wfl3*wfl1d)*hm + wfl3*wfl1*hmd
    fcdz5d = (wfr4d*wfr1+wfr4*wfr1d)*hp + wfr4*wfr1*hpd + (wfl4d*wfl1+&
&     wfl4*wfl1d)*hm + wfl4*wfl1*hmd
    flc1d = 0.5d0*sn*(nx*fcdx1d+ny*fcdy1d+nz*fcdz1d)
    flc2d = 0.5d0*sn*(nx*fcdx2d+ny*fcdy2d+nz*fcdz2d)
    flc3d = 0.5d0*sn*(nx*fcdx3d+ny*fcdy3d+nz*fcdz3d)
    flc4d = 0.5d0*sn*(nx*fcdx4d+ny*fcdy4d+nz*fcdz4d)
    flc5d = 0.5d0*sn*(nx*fcdx5d+ny*fcdy5d+nz*fcdz5d)
    flux1d(i) = flc1d
    flux2d(i) = flc2d
    flux3d(i) = flc3d
    flux4d(i) = flc4d
    flux5d(i) = flc5d
  END DO
END SUBROUTINE FLUXROE_D
