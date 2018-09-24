subroutine dfo2_d(dw, dwd, w, wd, im, gh)
  implicit none
  ! -------------------------------------------------
  integer,intent(in) :: im, gh
  real(8),dimension(1-gh:im+gh),intent(inout) :: w
  real(8),dimension(1-gh:im+gh),intent(inout) :: dw
  real(8),dimension(1-gh:im+gh),intent(inout) :: wd
  real(8),dimension(1-gh:im+gh),intent(inout) :: dwd
  integer :: i
  ! -------------------------------------------------
  
  !$DIR SIMD
  do i=1,im
    dwd(i) = 0.5d0*(wd(i+1) - wd(i-1))
  end do

end subroutine
