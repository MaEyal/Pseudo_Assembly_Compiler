; spass_error.as

MAIN: mov r1, LABEL1
.entry NONEXISTANTLABEL
jmp LABEL
bne &EX
.extern EX
