; file ps.as
;
;.entry LENGTH
;.extern W
;
;MAIN:		mov r3 ,LENGTH
;LOOP:		jmp L1(#-1,r6)
;  prn			#-5
;bne 		W(r4,r5)
;sub 		r1, r4
;bne 		L3
;L1: 		inc K
;    .entry LOOP
;bne LOOP(K,W)
;END: 		stop
;STR: 		.string "abcdef"
;LENGTH: 	.data 6,-9,15
;K: .data 22
;.extern L3

MAIN:	mov r3 ,LENGTH
LOOPY:	jmp L1(#-1,r6)
prn #-5
bne LOOP(r4,r7)
sub r1, r4
bne END
L1:		inc K
bne LOOPY(K,STR)
END: 	stop
STR: 	.string "abcdef"
LENGTH: .data 6,-9,15
K:		.data 22
