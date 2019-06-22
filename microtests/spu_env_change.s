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

; FF76 - pcm12
; FF77 - pcm34

; envelope 

/*
nr10 000 0 000          sweep off
nr11 10 001111          50% duty, sound length 15?
nr12 1111 0 000         initial volume 15, envelope decrease, no? envelope
nr14 10 xxx 00000000000 start sound, no length, high freq bit 0
nr12 0001 1 000         initial volume 1, envelope increase, no? envelope

nr14 00 xxx 00111110100

nr14 00 xxx 00111110100

nr14 00 xxx 00111110100
*/

main:

	; turn audio on
	ld a, %11111111
	ldh ($25), a
	ldh ($26), a

	; NR10 -PPPNSSS Sweep period, negate, shift
	ld a, %00000000
	ldh ($10), a

	; NR11 DDLLLLLL Duty, Length load (64-L)
	ld a, %10000000
	ldh ($11), a

	; NR12 VVVVAPPP Starting volume, Envelope add mode, period
	ld a, %11110001
	ldh ($12), a

	; NR13 FFFFFFFF Frequency LSB
	ld a, %11111111
	ldh ($13), a

	; NR14 TL---FFF Trigger, Length enable, Frequency MSB
	ld a, %10000111
	ldh ($14), a

	/*
	; NR12 VVVVAPPP Starting volume, Envelope add mode, period
	ld a, %11110111
	ldh ($12), a
	*/

end:
	ldh a, ($76)
	and $0F
	ld ($8000), a
	ldh a, ($26)
	and $0F
	ld ($8002), a
	jr end
