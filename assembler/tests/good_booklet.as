MAIN: 	mov @r3, LENGTH
LOOP: 	jmp L1
	  	prn -5
	  	bne LOOP
	  	sub @r1, @r4
	  	bne END
L1:	  	inc K
	  	bne LOOP
END:  	stop
STR:  	.string "abcdef"
LENTGH: .data 6,-9,15
K:		.data 22
