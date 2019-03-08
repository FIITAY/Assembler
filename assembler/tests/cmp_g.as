cmp 1, 1
cmp -4, @r1
cmp 5, LEN
cmp @r2, 1
cmp @r6, @r7
cmp @r4, LEN
cmp LEN, 100
cmp LEN3, @r6
cmp LEN, LEN3
cmp LEN, LEN1

LEN: .data 5
LEN1: .string "hello"
.extern LEN3
