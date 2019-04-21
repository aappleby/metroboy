.include "header.inc"

; When does the PPU latch scx?

; 5  - no scroll
; 6  - no scroll
; 7  - no scroll
; 8  - scroll
; 9  - scroll
; 10 - scroll

.define DELAY 8

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
	ld a, 4       ; 2
	ldh ($43), a  ; 3
	nops DELAY    ; DELAY
	ld a, 0       ; 2
	ldh ($43), a  ; 3
	reti
