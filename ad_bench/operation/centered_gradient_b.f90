!        Generated by TAPENADE     (INRIA, Ecuador team)
!  Tapenade 3.13 (r6666M) - 28 May 2018 09:28
!
!  Differentiation of centered_gradient in reverse (adjoint) mode:
!   gradient     of useful results: w dw
!   with respect to varying inputs: w dw
!   RW status of diff variables: w:incr dw:in-out
SUBROUTINE CENTERED_GRADIENT_B(w, wb, dw, dwb, n_cell, gh)
  IMPLICIT NONE
! -------------------------------------------------
  INTEGER, INTENT(IN) :: n_cell, gh
  REAL*8, DIMENSION(1-gh:n_cell+gh), INTENT(INOUT) :: w
  REAL*8, DIMENSION(1-gh:n_cell+gh), INTENT(INOUT) :: wb
  REAL*8, DIMENSION(1-gh:n_cell+gh), INTENT(INOUT) :: dw
  REAL*8, DIMENSION(1-gh:n_cell+gh), INTENT(INOUT) :: dwb
  INTEGER :: i
  wb = 0.0_8 ! TODO note: added by hand (but wasn't it removed by hand?)
  DO i=1,n_cell
    wb(i+1) = wb(i+1) + 0.5d0*dwb(i)
    wb(i-1) = wb(i-1) - 0.5d0*dwb(i)
  END DO
END SUBROUTINE CENTERED_GRADIENT_B
