integer function get_file_line( filename )
!
! get the number of input file line, written by allen W, 18-11-2014
!
implicit none

character ( len = * )  :: filename
integer                :: line_num

open ( 11, file = filename, status = 'old' )

line_num = 0

do while ( .true. )
read ( 11, *, end = 100 )
line_num = line_num + 1
end do
100 continue

close (11)

get_file_line = line_num

end function get_file_line

