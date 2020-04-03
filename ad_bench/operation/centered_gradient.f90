subroutine centered_gradient(w, dw, n_cell, gh)
  implicit none
  ! -------------------------------------------------
  integer,intent(in) :: n_cell, gh
  real(8),dimension(1-gh:n_cell+gh),intent(inout) :: w
  real(8),dimension(1-gh:n_cell+gh),intent(inout) :: dw
  integer :: i
  ! -------------------------------------------------
  ! write(*,*) "oooo"
  !$AD II-LOOP
  do i=1,n_cell
    dw(i) = 0.5d0*(w(i+1) - w(i-1))
  end do
end subroutine
