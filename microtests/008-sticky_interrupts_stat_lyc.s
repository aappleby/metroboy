.include "header.inc"

; Enabling lyc interrupt in STAT while in ly==lyc should NOT set the STAT bit in IF

; DMG         - bad
; AGB         - bad
; BGB         - bad
; Sameboy DMG - bad
; Sameboy AGB -
; Mooneye     - bad
; Tictocc     - bad

	; set lyc to 10
	ld a, $10
	ldh ($45), a

wait_lyc:
	ldh a, ($44)

	cp 10
	jr nz, wait_lyc

	; clear IF
	xor a
	ldh ($0F), a

	; enable stat lyc interrupt
	ld a, %01000000
	ldh ($41), a

	; store IF
	ldh a, ($0F)
	and %00011111

	; convert to dots
	add ($55 - $02)

loop:
	ld ($8000), a
	jr loop