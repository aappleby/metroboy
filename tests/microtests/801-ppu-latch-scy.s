.include "header.inc"

; When does the PPU latch scy?

; 5  - no scroll
; 6  - first column weird
; 7  - one scrolled column
; 8  - one scrolled column, second is weird
; 9  - two scrolled columns
; 10 - two scrolled columns, third is weird

.define DELAY 9

main:
	wait_vblank
	load_box

	; Enable the oam interrupt
	ld a, %00100000
	ldh ($41), a
	; Enable stat interrupts
	ld a, %00000010
	ldh ($FF), a
	ei

end:
	halt
	jr end

.org $48
	ld a, 4
	ldh ($42), a
	nops DELAY
	ld a, 0
	ldh ($42), a
	reti
