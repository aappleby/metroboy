.include "header.inc"

.define SCROLL 0

; ags overhead 70?
; 0 0 0 1 1 1 1 2

; dmg overhead 65
; 0 1 1 1 1 2 2 2

main:
	; Set scroll
	ld a, SCROLL
	ldh ($43), a

	; Halt until next OAM
	xor a
	ldh ($0F), a
	ld a, %00100000
	ldh ($41), a
	ld a, %00000010
	ldh ($FF), a
	ei
	halt
	nop

	; Start timer
	ld a, %00000101
	ldh ($07), a
	xor a
	ldh ($05), a
	ldh ($04), a

	; Halt until HBLANK
	xor a
	ldh ($0F), a
	ld a, %00001000
	ldh ($41), a
	ld a, %00000010
	ldh ($FF), a
	ei
	halt
	nop

	; Stop timer
	ld hl, $FF05
	xor a
	add (hl)
	nop
	add (hl)
	nop
	add (hl)
	nop
	add (hl)

	; Turn off scroll
	ld b, a
	ld a, 0
	ldh ($43), a
	ld a, b

end:
	ld ($8000), a
	jr end


.org $48
	ret