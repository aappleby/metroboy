.include "header.inc"

; When does the PPU latch bit 0 of LCDC (background enable)?

.define DELAY 9

main:
	wait_vblank
	load_box

	; Enable the oam interrupt
	ld a, %00100000
	ldh ($41), a
	; Enable stat interrupts
	ld a, %0000010
	ldh ($FF), a
	ei

end:
	halt
	jr end

.org $48
	ld a, $90 ; bg off
	ldh ($40), a
	nops DELAY
	ld a, $91 ; bg on
	ldh ($40), a
	reti
