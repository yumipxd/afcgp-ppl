program select_bond

!
!
!

implicit none

integer  :: i, j, k1, k2, nframe, FileLine, bondType, angleType, group_number, max_bond_number
logical, allocatable :: BondTerm(:), AngleTerm(:)
integer, allocatable :: bond_atom1(:), bond_atom2(:), angle_atom1(:), angle_atom2(:), angle_atom3(:)
real*8, allocatable  :: x(:), y(:), z(:)
real*8        :: dx, dy, dz, dd, v1x, v1y, v1z, v2x, v2y, v2z
character*10  :: skip1, nframe_C
character*6   :: signa(4)
character*50  :: parma(4), calAngle
character*120 :: File_Context, BondList, groupListFile
integer, external :: get_file_line
real*8,  external :: anglevector

call getarg(1, signa(1))
call getarg(2, parma(1))

call getarg(3, signa(2))
call getarg(4, parma(2))

call getarg(5, signa(3))
call getarg(6, parma(3))



do i = 1, 3
if (trim(adjustl(signa(i))) == "-f" .or. trim(adjustl(signa(i))) == "-F") then
BondList = parma(i)
else if (trim(adjustl(signa(i))) == "-g" .or. trim(adjustl(signa(i))) == "-G") then
groupListFile = parma(i)
else if (trim(adjustl(signa(i))) == "-n" .or. trim(adjustl(signa(i))) == "-N")then
nframe_C = parma(i)
read(nframe_C, '(i10)') nframe
else if (trim(adjustl(signa(i))) == "-h" .or. trim(adjustl(signa(i))) == "-H")then
write(*, 1111)
write(*, 1112)
write(*, 1113)
stop
else
write(*, 1110)
stop
end if
end do

group_number = get_file_line(groupListFile)

1110    format("Error input type, maybe you can try 'h or H' for more information")
1111    format("-f --> bond List file")
1112    format("-n --> numbers of frame")
1113    format("-g --> group List file")

FileLine = get_file_line(BondList)

open (112, file=BondList)

allocate (BondTerm(FileLine), AngleTerm(FileLine))
allocate (bond_atom1(FileLine), bond_atom2(FileLine))
allocate (angle_atom1(FileLine), angle_atom2(FileLine), angle_atom3(FileLine))

 BondTerm = .false.
AngleTerm = .false.

do i = 1, FileLine
read (112, '(a120)') File_Context
if (File_Context(1:4) == "BOND") BondTerm(i) = .true.
if (File_Context(1:5) == "ANGLE") AngleTerm(i) = .true.
end do

rewind(112)

 bondType = 0
angleType = 0

max_bond_number = 0

do i = 1, FileLine
if (BondTerm(i)) then 
 bondType =  bondType + 1
read (112, *) skip1, bond_atom1(bondType), bond_atom2(bondType)
if (bond_atom1(bondType) > max_bond_number) max_bond_number = bond_atom1(bondType)
if (bond_atom2(bondType) > max_bond_number) max_bond_number = bond_atom2(bondType)
else if (AngleTerm(i)) then
angleType = angleType + 1
read (112, *) skip1, angle_atom1(angleType), angle_atom2(angleType), angle_atom3(angleType)
if (angle_atom1(angleType) > max_bond_number) max_bond_number = angle_atom1(angleType)
if (angle_atom2(angleType) > max_bond_number) max_bond_number = angle_atom2(angleType)
if (angle_atom3(angleType) > max_bond_number) max_bond_number = angle_atom3(angleType)
else
read (112, *)
end if
end do

open (113, file = "CenterGroups.DAT")

open (116, file = "BondProfile.DAT")
open (117, file = "AngleProfile.DAT")

allocate(x(max_bond_number), y(max_bond_number), z(max_bond_number))

do i = 1, nframe

do j = 1, group_number
read (113, *) x(j), y(j), z(j)
end do

do k1 = 1, bondType
dx = x(bond_atom1(k1)) - x(bond_atom2(k1))
dy = y(bond_atom1(k1)) - y(bond_atom2(k1))
dz = z(bond_atom1(k1)) - z(bond_atom2(k1))
dd = dsqrt(dx**2 + dy**2 + dz**2)
write(116, '(2i7,f10.4)') bond_atom1(k1), bond_atom2(k1), dd
end do

do k1 = 1, angleType
v1x = x(angle_atom2(k1)) - x(angle_atom1(k1))
v2x = x(angle_atom2(k1)) - x(angle_atom3(k1))

v1y = y(angle_atom2(k1)) - y(angle_atom1(k1))
v2y = y(angle_atom2(k1)) - y(angle_atom3(k1))

v1z = z(angle_atom2(k1)) - z(angle_atom1(k1))
v2z = z(angle_atom2(k1)) - z(angle_atom3(k1))

write(calAngle, '(6f8.3)') v1x, v1y, v1z, v2x, v2y, v2z

write(117, '(3i7,f10.4)') angle_atom1(k1), angle_atom2(k1), angle_atom3(k1), anglevector(calAngle)
end do

end do
end program select_bond 
