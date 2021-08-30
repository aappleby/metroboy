.include "header.inc"

.define DELAY 132

; ags
;   0 - dots
;  17 - dots  (last cycle of oam line 0)

;  18 - white (first cycle of vram on line 0)
;  60 - white (last cycle of vram on line 0)

;  61 - dots  (first cycle of hblank on line 0)
; 110 - dots
; 111 - white (last cycle of line)

; 112 - white (first cycle of oam on line 1)
; 131 - white (last cycle of oam on line 1, no hole between oam and vram)

; 132 - white (first cycle of vram on line 1)
; 174 - white (last cycle of vram on line 1)

; 175 - dots  (first cycle of hblank on line 1)
; 224 - dots
; 225 - white (last cycle of hblank on line 1)

main:
	xor a
	ldh ($40), a
	ld ($FE00), a

	ld de, $FE00
	ld a, $91
	ldh ($40), a
.repeat DELAY
	nop
.endr
	ld (de), a

-	ldh a,($41)
	and 3
	cp 1
	jr nz, -

	ld a,($FE00)

-   ld ($8000), a
    jr -