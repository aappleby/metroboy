.include "header.inc"

; If DIV and TIMA are reset, we should be able to use four TIMA adds to measure
; things to the cycle.

.define OVERHEAD 9

main:
	; Start the fast timer
	ld a, %00000101
	ldh ($07), a
	ld h, $FF
	ld l, $05
	xor a
	ldh ($05), a
	ldh ($04), a

	; Measure things here
.repeat $55
	nop
.endr

	; Add up four TIMAs
	add (hl)
	nop
	add (hl)
	nop
	add (hl)
	nop
	add (hl)
	nop
	sub OVERHEAD

end:
	ld ($8000), a
	jr end