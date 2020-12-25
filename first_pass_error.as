; file fpass_errors.as
.data ,12, 19
.data 12,, -4
.data
.string "trust the "force" 	luke
MAIN: mov r2,r4
MAIN: rts
bne #12
sub r3
prn MAIN, #12
lea r3, r2
add ,r3, r2
mov #12, , MAIN
sub #3, #4
bad_instruction r2, MAIN
0badlabel: stop
THISLABELNAMEISWAYTOOLONGTOBEVALID: rts
r3: mov MAIN, r1
string: bne LABEL
sub: jmp &LABEL
.string	   		 	
sub   	 	 	 	
LABEL:
sub &LABEL , r3



