real*8 function anglevector( inpchar )
!
!calculate the angle between two different vectors, written by allen W,
!13-11-2014
!
implicit none

integer, external   :: getdatan
character( len = * ):: inpchar
real*8              :: x1, y1, z1, x2, y2, z2, dr1, dr2, drr, pi, bangle


pi     = acos(-1.0d0)
bangle = 180.0d0

if (getdatan(inpchar) /= 6 ) then
print*, "The number of your input digits for anglevector function isn't
6 !!"
stop
end if

read (inpchar, *) x1, y1, z1, x2, y2, z2

dr1 = dsqrt ( x1**2 + y1**2 + z1**2 )
dr2 = dsqrt ( x2**2 + y2**2 + z2**2 )

drr = x1*x2 + y1*y2 + z1*z2

if (abs(drr/dr1/dr2 - 1.0) <= 0.0001d0) then
anglevector = 0.0d0
else
anglevector = bangle * acos(drr/dr1/dr2)/ pi
end if

end function anglevector

