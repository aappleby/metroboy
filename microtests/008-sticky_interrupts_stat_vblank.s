.include "header.inc"

; Enabling vblank interrupt in STAT while in vblank should set the STAT bit in IF

; DMG         - dots
; AGB         - dots
; BGB         - bad
; Sameboy DMG - dots
; Sameboy AGB -
; Mooneye     - bad
; Tictocc     - bad

wait_vblank:
	ldh a, ($41)
	and $03
	cp $01
	jr nz, wait_vblank

	; clear IF
	xor a
	ldh ($0F), a

	; enable stat vsync interrupt
	ld a, %00010000
	ldh ($41), a

	; store IF
	ldh a, ($0F)
	and %00011111

	; convert to dots
	add ($55 - $02)

loop:
	ld ($8000), a
	jr loop