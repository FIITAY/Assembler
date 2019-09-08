.entry LENGTH
.extern W
MAIN: mov @r3 , LENGTH
LOOP: jmp MAIN
prn -5
bne W
STR: .string "abcdef"
LENGTH: .data 6,-9,15
