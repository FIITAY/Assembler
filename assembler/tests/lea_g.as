; target is register.
lea LEN, @r3


; target is memory location
lea   LEN, LEN4
lea LEN, LEN

LEN: .data 5
.extern LEN4

