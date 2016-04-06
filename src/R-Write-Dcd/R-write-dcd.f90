program R_write_dcd
implicit none

integer  :: nf, i, natoms, dcdfileSer, nframe, eframe, bframe, bin
real*8, allocatable :: x1(:), y1(:), z1(:)

character*50 :: dcdfilename

character*2  :: signa(4)
character*20 :: parma(4), eframe_C, bframe_C, bin_C

bframe = 1
eframe = nframe
   bin = 1

call getarg(1, signa(1))
call getarg(2, parma(1))

call getarg(3, signa(2))
call getarg(4, parma(2))

call getarg(5, signa(3))
call getarg(6, parma(3))

call getarg(7, signa(4))
call getarg(8, parma(4))

do i = 1, 4
if( trim(signa(i))== "-f".or. trim(signa(i))== "-F") then
  dcdfilename = parma(i)
else if( trim(signa(i))== "-b".or. trim(signa(i))== "-B") then
  bframe_C = parma(i)
  read(bframe_C, '(i9)') bframe
else if( trim(signa(i))== "-d".or. trim(signa(i))== "-D") then
     bin_C = parma(i)
  read(bin_C,'(i9)') bin
else 
  write(*,1111)
  stop
end if
end do


1111    format ("<-f>[dcd-file-name] <-b>[first-frame-to-read] <-e>[last-frame-to-read]")

!!! *** get coordinate from the dcd file

open (19, file="SelectFrame.XYZ")

call Load_dcd_head(dcdfilename, nframe, natoms, dcdfileSer)

eframe = nframe

do i = 1, 4
if( trim(signa(i))== "-e".or. trim(signa(i))== "-E") then
  eframe_C = parma(i)
  read(eframe_C, '(i9)') eframe
end if
end do

allocate ( x1(natoms), y1(natoms), z1(natoms) )

if (eframe > nframe) then
write(*,*)"Warning :: The frames should less than total frames"
eframe = nframe
end if

write(19, *) natoms, (eframe-bframe+1)/bin

do nf = 1, eframe, bin
call Load_dcd_coor(dcdfileSer, natoms, x1, y1, z1)
if (nf >= bframe) then
do i = 1, natoms
write(19, '(3f10.4)') x1(i), y1(i), z1(i)
end do
end if
end do


close(19)
end program R_write_dcd
