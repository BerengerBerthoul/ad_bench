subroutine dfo2(dw, w, im, gh)
  implicit none
  ! -------------------------------------------------
  integer,intent(in) :: im, gh
  real(8),dimension(1-gh:im+gh),intent(inout) :: w
  real(8),dimension(1-gh:im+gh),intent(inout) :: dw
  integer :: i
  ! -------------------------------------------------
  ! write(*,*) "oooo"
  !$AD II-LOOP
  do i=1,im
    dw(i) = 0.5d0*(w(i+1) - w(i-1))
  end do

end subroutine
