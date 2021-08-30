.include "header.inc"

; When, relative to the OAM interrupt, does vram become locked?

; 3  - dots
; 4  - no dots

.define DELAY 3

main:
	wait_vblank

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
	ld a, $00		; 2
	ld ($8000), a   ; 4
	nops DELAY      ; DELAY
	ld a, $55       ; 2
	ld ($8000), a   ; 4
	reti

.org $40
	reti