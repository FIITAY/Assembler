; target is register.
sub 4, @r1
sub LEN, @r3
sub @r2, @r2

; target is memory location
sub     @r7 ,   LEN
sub   67, LEN
sub   LEN, LEN4
sub LEN, LEN

LEN: .data 5
.extern LEN4

