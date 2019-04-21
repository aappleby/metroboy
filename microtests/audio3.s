.include "header.inc"


; Wave
; NR30 FF1A E--- ---- DAC power
; NR31 FF1B LLLL LLLL Length load (256-L)
; NR32 FF1C -VV- ---- Volume code (00=0%, 01=100%, 10=50%, 11=25%)
; NR33 FF1D FFFF FFFF Frequency LSB
; NR34 FF1E TL-- -FFF Trigger, Length enable, Frequency MSB

; DAC power
; Length
; Volume
; Frequency LSB
; Frequency MSB - can change 
; Length enable
; Trigger

main:

	; turn audio on
	ld a, %11111111
	ldh ($25), a
	ldh ($26), a

	; load waveform
	ld a, $FF
	ldh ($30), a
	ldh ($31), a
	ldh ($32), a
	ldh ($33), a
	ldh ($34), a
	ldh ($35), a
	ldh ($36), a
	ldh ($37), a
	ld a, $00
	ldh ($38), a
	ldh ($39), a
	ldh ($3A), a
	ldh ($3B), a
	ldh ($3C), a
	ldh ($3D), a
	ldh ($3E), a
	ldh ($3F), a

	; FF1A E------- DAC power
	ld a, %10000000
	ldh ($1A), a

	; FF1B LLLLLLLL Length load (256-L)
	ld a, %11111111
	ldh ($1B), a

	; FF1C -VV----- Volume code (00=0%, 01=100%, 10=50%, 11=25%)
	ld a, %00100000
	ldh ($1C), a
	
	; FF1D FFFFFFFF Frequency LSB
	ld a, %11111111
	ldh ($1D), a
	
	; FF1E TL---FFF Trigger, Length enable, Frequency MSB
	ld a, %11000011
	ldh ($1E), a

	; FF1A E------- DAC power
	ld a, %00000000
	ldh ($1A), a

end:
	jr end
