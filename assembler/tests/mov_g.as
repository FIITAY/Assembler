; target is register.
mov 4, @r1
mov LEN, @r3
mov @r2, @r2

; target is memory location
mov     @r7 ,   LEN
mov   67, LEN
mov   LEN, LEN4
mov LEN, LEN

LEN: .data 5
.extern LEN4

