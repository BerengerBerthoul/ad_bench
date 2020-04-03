subroutine centered_gradient_d(w, wd, dw, dwd, n_cell, gh)
  implicit none
  ! -------------------------------------------------
  integer,intent(in) :: n_cell, gh
  real(8),dimension(1-gh:n_cell+gh),intent(inout) :: w
  real(8),dimension(1-gh:n_cell+gh),intent(inout) :: dw
  real(8),dimension(1-gh:n_cell+gh),intent(inout) :: wd
  real(8),dimension(1-gh:n_cell+gh),intent(inout) :: dwd
  integer :: i
  ! -------------------------------------------------
  
  !$DIR SIMD
  do i=1,n_cell
    dwd(i) = 0.5d0*(wd(i+1) - wd(i-1))
  end do
end subroutine
