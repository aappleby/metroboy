.include "header.inc"

main:
	; turn all interrupt sources on
	ld a, $FF
	ldh ($FF), a

	; clear IF
	xor a
	ldh ($0F), a

wait_vblank:
	ldh a, ($44)
	cp $90
	jr nz, wait_vblank

	; clear IF and IE
	xor a
	ldh ($0F), a
	ldh ($FF), a

	; turn IE back on
	dec a
	ldh ($FF), a

	; store IF
	ldh a, ($0F)
	and %00011111

loop:
	ld ($8000), a
	jr loop