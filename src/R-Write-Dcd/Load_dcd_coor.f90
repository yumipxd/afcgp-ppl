subroutine Load_dcd_coor(file_ser, nnatoms, xx, yy, zz)
implicit none

integer :: nnatoms, i, file_ser
real*8  :: lbox(6), xx(nnatoms), yy(nnatoms), zz(nnatoms)

read (file_ser) (lbox(i), i = 1, 6)
read (file_ser) (xx(i), i = 1, nnatoms)
read (file_ser) (yy(i), i = 1, nnatoms)
read (file_ser) (zz(i), i = 1, nnatoms)

end subroutine Load_dcd_coor
