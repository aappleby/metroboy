.include "header.inc"

; correct -
; 54 - black
; 55 - white

; ticktocc -
; 53 - black
; 54 - white

.define DELAY 54

main:
	; wait for hblank on line 1
    di
-	ldh a,($44)
	cp 10
	jr nz, -
-	ldh a,($41)
	and 3
	jr nz, -

	; clear the first byte of oam
	xor a
	ld ($FE00), a

	; switch to oam int mode
	ld a, %00100000
	ldh ($41), a
	ld a, %00000010
	ldh ($FF), a

	; clear interrupts
	xor a
	ldh ($0F), a

	; wait for oam interrupt
	ei
	halt
	nop

-	jr -

.org $48
.repeat DELAY
	nop
.endr

	; load first byte of oam (if possible)
	ld a,($FE00)

	; and put it on screen
-	ld ($8000), a		; 4 cycles, write happens on last cycle
	jr -