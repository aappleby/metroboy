.include "header.inc"


; Square 1
; NR10 FF10 -PPPNSSS Sweep period, negate, shift
; NR11 FF11 DDLLLLLL Duty, Length load (64-L)
; NR12 FF12 VVVVAPPP Starting volume, Envelope add mode, period
; NR13 FF13 FFFFFFFF Frequency LSB
; NR14 FF14 TL---FFF Trigger, Length enable, Frequency MSB

; FF76 - pcm12
; FF77 - pcm34

; envelope 

.macro nr10 ARGS p n s
	ld a, (p << 4) | (n << 3) | s
	ldh ($10), a
.endm

.macro nr11 ARGS d l
	ld a, (d << 6) | l
	ldh ($11), a
.endm

.macro nr12 ARGS v a p
	ld a, (v << 4) | (a << 3) | p
	ldh ($12), a
.endm

.macro trigger ARGS freq l
	ld a, freq & $FF
	ldh ($13), a
	ld a, $80 | (l << 6) | (freq >> 8)
	ldh ($14), a
.endm

;-------------------------------------------------------------------------------

main:
	ld a, $FF
	ldh ($25), a
	ldh ($26), a

	nr10 0 0 0
	nr11 2 0

	nr12 15 0 0

	; 18
	trigger (2047 - 47) 0

	;nr12 10 1 0
	;nr12 10 0 0

end:
	ldh a, ($76)
	and $0F
	ld ($8000), a
	ldh a, ($26)
	and $0F
	ld ($8002), a
	jr end
