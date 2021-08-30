.include "header.inc"

; When does the PPU latch bit 4 of LCDC (tile select)?

.define DELAY 15

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
	ld a, $91 ; select $8000 for tiles
	ldh ($40), a
	nops DELAY
	ld a, $81 ; select $8800 for tiles
	ldh ($40), a
	reti
