.include "header.inc"


; Square 2
; NR21 FF16 DDLLLLLL Duty, Length load (64-L)
; NR22 FF17 VVVVAPPP Starting volume, Envelope add mode, period
; NR23 FF18 FFFFFFFF Frequency LSB
; NR24 FF19 TL---FFF Trigger, Length enable, Frequency MSB

main:

	; turn audio on
	ld a, %11111111
	ldh ($25), a
	ldh ($26), a

	; FF16 DDLLLLLL Duty, Length load (64-L)
	ld a, %10000000
	ldh ($16), a

	; FF17 VVVVAPPP Starting volume, Envelope add mode, period
	ld a, %11110000
	ldh ($17), a

	; FF18 FFFFFFFF Frequency LSB
	ld a, %01111111
	ldh ($18), a

	; FF19 TL---FFF Trigger, Length enable, Frequency MSB
	ld a, %10000111
	ldh ($19), a

end:
	jr end
