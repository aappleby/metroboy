.include "header.inc"

; We can read vram during dma
; We can write vram during dma
; rom reads as 0xFF
; external ram reads as 0xFF
; work ram reads as 0xFF
; echo reads as 0xFF
; oam reads as 0xFF

main:
	wait_vblank

	ld a, %10010011
	ldh ($40), a

	ld a, %11100100
	ldh ($48), a

	ld hl, $8010
	ld a, $FF
	ld (hl+), a
	ld (hl+), a
	ld (hl+), a
	ld (hl+), a
	ld (hl+), a
	ld (hl+), a
	ld (hl+), a
	ld (hl+), a
	ld (hl+), a
	ld (hl+), a
	ld (hl+), a
	ld (hl+), a
	ld (hl+), a
	ld (hl+), a
	ld (hl+), a
	ld (hl+), a

	ld hl, dma_func
	ld de, $FF80
	ld b, dma_func_end - dma_func

copy_loop:
	ld a, (hl+)
	ld (de), a
	inc de
	dec b
	jr nz, copy_loop

	ld a, $02
	call $FF80

end:
	jr end

dma_func:
	ldh ($46), a
	nop
	nop
	nop
	nop
	nop
	nop

	;ld a, $55
	;ld ($8000), a

	ld c, $26
dma_delay_loop:
	dec c
	jr nz, dma_delay_loop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	ret
dma_func_end:

.org $0200
.db $10, 10,  $01, %01100000,
.db $10, 20,  $01, %01100000,
.db $10, 30,  $01, %01100000,
.db $10, 40,  $01, %01100000,
.db $10, 50,  $01, %01100000,
.db $10, 60,  $01, %01100000,
.db $10, 70,  $01, %01100000,
.db $10, 80,  $01, %01100000,
.db $10, 90,  $01, %01100000,
.db $10, 100, $01, %01100000,

.db $20, 10,  $01, %01100000,
.db $20, 20,  $01, %01100000,
.db $20, 30,  $01, %01100000,
.db $20, 40,  $01, %01100000,
.db $20, 50,  $01, %01100000,
.db $20, 60,  $01, %01100000,
.db $20, 70,  $01, %01100000,
.db $20, 80,  $01, %01100000,
.db $20, 90,  $01, %01100000,
.db $20, 100, $01, %01100000,

.db $30, 10,  $01, %01100000,
.db $30, 20,  $01, %01100000,
.db $30, 30,  $01, %01100000,
.db $30, 40,  $01, %01100000,
.db $30, 50,  $01, %01100000,
.db $30, 60,  $01, %01100000,
.db $30, 70,  $01, %01100000,
.db $30, 80,  $01, %01100000,
.db $30, 90,  $01, %01100000,
.db $30, 100, $01, %01100000,

.db $40, 10,  $01, %01100000,
.db $40, 20,  $01, %01100000,
.db $40, 30,  $01, %01100000,
.db $40, 40,  $01, %01100000,
.db $40, 50,  $01, %01100000,
.db $40, 60,  $01, %01100000,
.db $40, 70,  $01, %01100000,
.db $40, 80,  $01, %01100000,
.db $40, 90,  $01, %01100000,
.db $40, 100, $01, %01100000,

.db $FF, $FF, $FF, $FF,
.db $FF, $FF, $FF, $FF,
.db $FF, $FF, $FF, $FF,
.db $FF, $FF, $FF, $FF,
.db $FF, $FF, $FF, $FF,
.db $FF, $FF, $FF, $FF,
.db $FF, $FF, $FF, $FF,
.db $FF, $FF, $FF, $FF,
.db $FF, $FF, $FF, $FF,
.db $FF, $FF, $FF, $FF,