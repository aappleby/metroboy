.include "header.inc"


; Square 1
; NR10 FF10 -PPPNSSS Sweep period, negate, shift
; NR11 FF11 DDLLLLLL Duty, Length load (64-L)
; NR12 FF12 VVVVAPPP Starting volume, Envelope add mode, period
; NR13 FF13 FFFFFFFF Frequency LSB
; NR14 FF14 TL---FFF Trigger, Length enable, Frequency MSB

; starting with sweep off then turning it on after trigger does something weird

; starting with %01110000 and changing to %01110111 after trigger is super weird
; starting with %01111000 and changing to %01111111 after trigger also super weird, sounds like above

main:

	; turn audio on
	ld a, %11111111
	ldh ($25), a
	ldh ($26), a

	; FF10 -PPPNSSS Sweep period, negate, shift
	ld a, %01111000
	ldh ($10), a

	; FF11 DDLLLLLL Duty, Length load (64-L)
	ld a, %10000000
	ldh ($11), a

	; FF12 VVVVAPPP Starting volume, Envelope add mode, period
	ld a, %11110000
	ldh ($12), a

	; FF13 FFFFFFFF Frequency LSB
	ld a, %01111111
	ldh ($13), a

	; FF14 TL---FFF Trigger, Length enable, Frequency MSB
	ld a, %11000111
	ldh ($14), a

	; FF10 -PPPNSSS Sweep period, negate, shift
	ld a, %01111111
	ldh ($10), a

end:
	jr end
