.include "header.inc"

;-------------------------------------------------------------------------------

; dmg - 19, no dots. 20, dots.

.ifdef DMG

;   0 - stat 10000101
;   6 - stat 10000101
;   7 - stat 10000100 - glitch stat reads as hblank?

;   8 - stat 10000110 - oam line 0 starts here
;  27 - stat 10000110 - oam line 0 ends here

;  28 - stat 10000111 - vram line 0 starts here
;  70 - stat 10000111 - vram line 0 ends here

;  71 - stat 10000100 - hblank line 0 starts here
; 121 - stat 10000000 - lyc goes 0 on last cycle of hblank

; 122 - stat 10000010 - oam line 1 starts here
; 141 - stat 10000010 - oam line 1 ends here
; 142 - stat 10000011 - vram line 1 starts here
; 184 - stat 10000011 - vram line 1 ends here
; 185 - stat 10000000 - hblank line 1 starts here
; 235 - stat 10000000 - hblank line 1 ends here

; 236 - stat 10000010 - oam line 2 starts here

;   0 - ly 0 ?
;   6 - ly 0 ?

;   7 - ly 0
;   8 - ly 0
; 120 - ly 0

; 121 - ly 1
; 122 - ly 1
; 125 - ly 1

; 234 - ly 1
; 235 - ly 2

;-------------------------------------------------------------------------------

; tictocc
;  12 - stat 10000000
;  13 - stat 10000001

;   0 - ly 10011001
;   7 - ly 10011001
;   8 - ly 00000000

;   0 - stat 10000001
;   7 - stat 10000001
;   8 - stat 10000110
;  27 - stat 10000110

;  28 - stat 10000111
;  70 - stat 10000111

;  71 - stat 10000100
; 121 - stat 10000100, ly 0 (should be no lyc & ly = 1)

; 122 - stat 10000010, ly 1
; 123 - stat 10000010
; 141 - stat 10000010
; 142 - stat 10000011
; 184 - stat 10000011
; 185 - stat 10000000
; 235 - stat 10000000

; 236 - stat 10000010

; 121 - ly 0
; 122 - ly 1
; 123 - ly 1
; 125 - ly 1

.define DELAY 71
.endif


;-------------------------------------------------------------------------------
.ifdef AGS

; 475 - stat 10000001
; 476 - stat 10000101

; 587 - stat 10000101
; 588 - stat 10000110

;   0 - stat 10000001 ly 10010101
; 587 - stat 10000101 ly 00000000
; 588 - stat 10000110 ly 00000000

;   0 - ly reads as 148
; 474 - ly reads as 153
; 475 - ly reads as 0

.define DELAY 0
.endif

;-------------------------------------------------------------------------------

main:

.repeat DELAY
	nop
.endr

	ldh a, ($41)

end:
	ld ($8000), a		; 4 cycles, write happens on last cycle
	jr end