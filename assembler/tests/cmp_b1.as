; illigal register
cmp @r9, 4
cmp @r10, 4
cmp r2, 4

; illigal 1 param
WHO: cmp LEN

; illigal 3 params and more
cmp @r1, @r3, 4
cmp LEN, 4, 7, LEN
cmp LEN , , ,, 5
LEN: .data 5
.extern LEN4

;wrong command (added char)
cmpe @r2, @r3