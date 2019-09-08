; target is register.
add 4, @r1
add LEN, @r3
add @r2, @r2

; target is memory location
add     @r7 ,   LEN
add   67, LEN
add   LEN, LEN4
add LEN, LEN

LEN: .data 5
.extern LEN4

