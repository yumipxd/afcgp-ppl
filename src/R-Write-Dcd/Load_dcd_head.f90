subroutine Load_dcd_head(filename, total_frame, naatoms, fileser)
implicit none

integer :: total_frame, naatoms, ntitle, j, fileser
integer,dimension(20) :: ictr
character(len=80),dimension(100) :: title
character(len=4) :: sign1
real*4  :: delta
character*50     :: filename

fileser = 5001

open (unit=fileser, file=trim(filename), form='unformatted', status='unknown')

ntitle = 2
read(fileser) sign1,(ictr(j),j=1,9), delta, (ictr(j),j=11,20)
read(fileser) ntitle,(title(j),j=1,ntitle)
read(fileser) naatoms

total_frame = ictr(1)

end subroutine Load_dcd_head
