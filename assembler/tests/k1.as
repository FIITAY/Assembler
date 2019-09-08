.entry LENGTH
MAIN: mov @r3, LENGTH
LOOP: jmp MAIN
STR: .string "abcdef"
LENGTH: .data 6, -9, 15
