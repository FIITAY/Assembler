; illigal register
mov @r9, 4
mov @r10, 4
mov r2, 4

; illigal immidiate as target
mov     LEN ,   7

; illigal 1 param
WHO: mov LEN

; illigal 3 params and more
mov @r1, @r3, 4
mov LEN, 4, 7, LEN
mov LEN , , ,, 5
LEN: .data 5
.extern LEN4

;wrong command (added char)
move @r2, @r3

