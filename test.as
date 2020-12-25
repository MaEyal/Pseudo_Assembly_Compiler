; file official.as

mov #-5, 		 MAIN
mov #5, r3
LABEL2: 		mov   LABEL2    ,      MAIN
mov LABEL2 ,	  r7
mov r3, MAIN
mov r4, r1
cmp #12, #9
cmp #12, 	MAIN
cmp #12, r5
cmp LABEL2, #19
cmp LABEL2, LABEL2
cmp LABEL2, r4
MAIN:                                       cmp r2, #112
cmp r2, EX
cmp r2, r5
add 	#4, EX
add #4, 	r2
add LABEL2, MAIN
add LABEL2, r7
add r1    ,    LABEL2
.data -5, 12, 100
LABEL3: add r1, r5
sub #19, LABEL3
sub #19, r2
sub LABEL2, LABEL3
sub LABEL3, r3
.string               "hello"
sub r3, LABEL3
sub r2, r5
lea MAIN, LABEL2
lea MAIN, 		 r2
clr LABEL2
clr r0
not LABEL3
not r7
inc MAIN
inc r3
.data 9, 	-4    , 	19
dec LABEL3
dec r5
jmp MAIN
jmp &LABEL2
bne LABEL3
bne &LABEL3
          		jsr MAIN
jsr &MAIN
red LABEL2
red r1
prn #7
prn LABEL3
prn r7
rts
stop

.entry MAIN
.extern EX 
 
