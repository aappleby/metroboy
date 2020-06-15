.include "header.inc"


; Noise
;      FF1F ---- ---- Not used
; NR41 FF20 --LL LLLL Length load (64-L)
; NR42 FF21 VVVV APPP Starting volume, Envelope add mode, period
; NR43 FF22 SSSS WDDD Clock shift, Width mode of LFSR, Divisor code
; NR44 FF23 TL-- ---- Trigger, Length enable


; Noise
;      FF1F ---- ---- Not used
; NR42 FF21 VVVV APPP Starting volume, Envelope add mode, period
; NR43 FF22 SSSS WDDD Clock shift, Width mode of LFSR, Divisor code
; NR44 FF23 TL-- ---- Trigger, Length enable



; noise -
; reloading length register does _not_ extend sound
; continuously retriggering stops the sound
; continuously setting length enable bit stops the sound
; can change shift while noise is playing

; if length enabled
;    continuous
;    	length - writable (63-L-1)
;    	volume - is weird
;    	env direction
;    	env period
;    	shift
;    	width
;    	divisor
;    once
;    	length - writable (63-L-1)
;    	volume - is weird
;    	env direction - can't change
;    	env period
;    	shift
;    	width
;    	divisor


; if length not enabled
;    length
;    volume
;    env direction
;    env period
;    shift
;    width
;    divisor

; cannot change volume during sound at all?
; cannot increase volume during playback?
; can change env period during sound
; cannot change env dir during sound

; length enabled, once -
;  1: 
; 62: 
; 63: ........ ........ ........ ........ s....... ........ ........ ....xxxx
; seems like writing once when env is 63 clears bits (xor)

; length is always writable, but actual sound length is _63_-_L_-_1_, wrapping around to 63 if L = 63

; can change dir/period during sound, but still acts weird
; if volume = 0 and env = down, sound never starts



main:

	; turn audio on
	ld a, %11111111
	ldh ($25), a
	ldh ($26), a

	; FF20 --LLLLLL Length load (63-L-1)
	ld a, %00000000
	ldh ($20), a
	
	; FF21 VVVVAPPP Starting volume, Envelope add mode, period
	ld a, %11110000
	ldh ($21), a
	
	; FF22 SSSSWDDD Clock shift, Width mode of LFSR, Divisor code
	ld a, %00100111
	ldh ($22), a

	; FF23 TL------ Trigger, Length enable	
	ld a, %10000000
	ldh ($23), a

	; FF23 TL------ Trigger, Length enable	
	ld a, %11000000
	ldh ($23), a

end:
	jr end
