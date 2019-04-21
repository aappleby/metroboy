.include "header.inc"

; Enabling hblank interrupt in STAT while in hblank should set the STAT bit in IF

; DMG         - dots
; AGB         - dots
; BGB         - dots
; Sameboy DMG - dots
; Sameboy AGB -
; Mooneye     - bad
; Tictocc     - bad

wait_hblank:
	ldh a, ($41)
	and $03
	cp $00
	jr nz, wait_hblank

	; clear IF
	xor a
	ldh ($0F), a

	; enable stat hblank interrupt
	ld a, %00001000
	ldh ($41), a

	; store IF
	ldh a, ($0F)
	and %00011111

	; convert to dots
	add ($55 - $02)

loop:
	ld ($8000), a
	jr loop