.include "header.inc"

;-------------------------------------------------------------------------------

.ifdef DMG

; dmg

;   0 - 01100010 ? oam not clean on boot?
;   5 - 11111000
;   6 - black
;   7 - black
;   8 - black
;  28 - black
;  69 - black
;  70 - garbage
; 106 - garbage

;.define DELAY 69

;-------------------------------------------------------------------------------

; tictocc

;   0 - white
;   5 - white
;   6 - black
;  68 - black
;  69 - white? (should be black)
;  70 - white

.define DELAY 69
.endif


;-------------------------------------------------------------------------------
.ifdef AGS

; 585 - white
; 586 - black
; 649 - black
; 650 - white

.define DELAY 649
.endif

;-------------------------------------------------------------------------------

main:

.repeat DELAY
	nop
.endr

	ld a, ($FE00)       ; 4 cycles, read happes on last cycle

end:
	ld ($8000), a		; 4 cycles, write happens on last cycle
	jr end