.include "header.inc"

; Enabling oam interrupt in STAT while in oam should set the STAT bit in IF
; BUT ONLY ON DMG-01

; DMG         - dots
; AGB         - bad
; BGB         - dots
; Sameboy DMG - dots
; Sameboy AGB - bad
; Mooneye     - bad
; Tictocc     - bad

wait_oam:
	ldh a, ($41)
	and $03
	cp $02
	jr nz, wait_oam

	; clear IF
	xor a
	ldh ($0F), a

	; enable stat oam interrupt
	ld a, %00100000
	ldh ($41), a

	; store IF
	ldh a, ($0F)
	and %00011111

	; convert to dots
	add ($55 - $02)

loop:
	ld ($8000), a
	jr loop