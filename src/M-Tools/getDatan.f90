integer function getDatan( cStr )

character( len = * ) , intent( in ) :: cStr
integer :: i
logical :: bisSeparator , bisQuote
getDatan = 0
bisSeparator = .true.
bisQuote = .false.

do i = 1 , len_trim( cStr )
    Select Case( cStr(i:i) )
case( '"' , "'" )

if ( .not.bIsQuote ) getDatan = getDatan + 1

bisQuote = .not.bisQuote
bisSeparator = .false.

case( " " , "," , char(9) )

if ( .not.bIsQuote ) then
bisSeparator = .true.
end if

case default

  if ( bisSeparator ) then
        getDatan = getDatan + 1
  end if

bisSeparator = .false.
end select

end do
end function getDatan

