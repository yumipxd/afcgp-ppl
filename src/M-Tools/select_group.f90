program select_groups

!
!
!

implicit none

integer  :: i, j, k1, k2, skip1, natoms, inter_natoms, nframe, max_num, groupListLine
integer, allocatable :: group_num(:), group_serials(:)
real*8,  allocatable :: x(:), y(:), z(:), xc(:), yc(:), zc(:)
real*8,  allocatable :: energy1(:), energy2(:), energy_vdw(:), energy_ele(:)
character*50  :: parma(4), groupListFile, XyzFile
character*6   :: signa(4)
character*10  :: group_name, nframe_C
character*180 :: group_context
integer, external :: get_file_line, getDatan

nframe = 100

call getarg(1, signa(1))
call getarg(2, parma(1))

call getarg(3, signa(2))
call getarg(4, parma(2))

call getarg(5, signa(3))
call getarg(6, parma(4))

do i = 1, 3
if (trim(adjustl(signa(i))) == "-g" .or. trim(adjustl(signa(i))) == "-G") then
groupListFile = parma(i)
else if (trim(adjustl(signa(i))) == "-x" .or. trim(adjustl(signa(i))) == "-X") then
XyzFile = parma(i)
else if (trim(adjustl(signa(i))) == "-n" .or. trim(adjustl(signa(i))) == "-N")then
nframe_C = parma(i)
read(nframe_C, '(i10)') nframe
else if (trim(adjustl(signa(i))) == "-h" .or. trim(adjustl(signa(i))) == "-H") then
write(*, 1111)
write(*, 1112)
write(*, 1113)
stop
else
write(*, 1110)
stop
end if
end do

1110    format("Error input type, maybe you can try 'h or H' for more information")
1111    format("-g --> group List file") 
1112    format("-x --> xyz format file name")
1113    format("-n --> numbers of frame")

groupListLine = get_file_line(groupListFile)

allocate (group_num(groupListLine))

open (111, file=groupListFile)

max_num = 0

do i = 1, groupListLine
read (111, '(a180)') group_context
group_num(i) = getDatan(group_context)
if (group_num(i) > max_num) max_num = group_num(i)
end do

allocate (group_serials(max_num))

rewind(111)

allocate(xc(groupListLine), yc(groupListLine), zc(groupListLine))
allocate(energy_vdw(groupListLine), energy_ele(groupListLine))
do i = 1, groupListLine
read (111, *) group_name, (group_serials(j), j = 1, group_num(i)-1)
end do

open (112, file=XyzFile)

open (113, file="CenterGroups.DAT")
open (114, file="energyVdw.DAT")
open (115, file="energyEle.DAT")

read (112, *) natoms
read (114, *) inter_natoms
read (115, *)
allocate(x(natoms), y(natoms), z(natoms))
allocate(energy1(inter_natoms), energy2(inter_natoms))

xc = 0.0d0
yc = 0.0d0
zc = 0.0d0

do i = 1, nframe
do j = 1, natoms
read (112, *) x(j), y(j), z(j)
!-*-*-*-*-
do k1 = 1, groupListLine
do k2 = 1, group_num(k1)
if (group_serials(k2) == j) then
xc(k1) = xc(k1) + x(j)
yc(k1) = yc(k1) + y(j)
zc(k1) = zc(k1) + z(j)
end if
end do
end do
!-*-*-*-*-
end do

do j = 1, inter_natoms
read (114, *) skip1, energy1(j)
read (115, *) skip1, energy2(j)

do k1 = 1, groupListLine
do k2 = 1, group_num(k1)
if (group_serials(k2) == j) then
energy_vdw(k1) = energy_vdw(k1) + energy1(j)
energy_ele(k1) = energy_ele(k1) + energy2(j)
end if
end do
end do

end do

do k1 = 1, groupListLine
write (113, '(5f10.4)') xc(k1)/group_num(k1), yc(k1)/group_num(k1), zc(k1)/group_num(k1), energy_vdw(k1), energy_ele(k1)
xc(k1) = 0.0d0
yc(k1) = 0.0d0
zc(k1) = 0.0d0
energy_vdw(k1) = 0.0d0
energy_ele(k1) = 0.0d0
end do

end do


end program select_groups
