.entry LENGTH
.extern W
MAIN: mov @r3 , LENGTH
LOOP: jmp MAIN
prn -5
bne W
sub @r1, @r4
bne L1
L1: inc W
STR: .string "abcdef"
LENGTH: .data 6,-9,15
