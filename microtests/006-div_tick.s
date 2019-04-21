.include "header.inc"

; DIV should stay at $XX for N cycles.

.ifdef AGS
	.define NOPS 25
	.define PASS 39
.endif

.ifdef DMG
	.define NOPS 6
	.define PASS 172
.endif

main:
.repeat NOPS
 	nop
.endr
	ldh a, ($04)
	add $55 - PASS
end:
	ld ($8000), a
	jr end
