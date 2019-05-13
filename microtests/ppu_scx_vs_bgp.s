.include "header.inc"

;0->1 nada
;0->2 nada

; cycles 21 & 22 are weird

.define DELAY 20
main:
  lcd_off_unsafe

  ld a, 40
  ld hl, $9800 + 12
  ld bc, 32
.repeat 18
  ld (hl), a
  add hl, bc
.endr

  ld a, %11100100
  ldh (BGP), a

load_tile:
  ld bc, tile
  ld hl, $8000h
.repeat 16
  ld a, (bc)
  ld (hl+), a
  inc bc
.endr

  ld a, %10010001
  ldh (LCDC), a        ; 2
  nops 2

  ; 000:18 - 110 cycles during line 0 to set up the frame
frame_start:
  ld hl, $FF43         ; 3
  xor a                ; 1
  ld (hl), a           ; 2

  ld a, 0              ; 2
  ld b, 0              ; 2
  ld c, 0              ; 2
  ld d, 0              ; 2
  ld e, 143            ; 2
  nops 94

  ; 001:2 - 110 cycles to mess with the line.
line_loop:
  ld (hl), a
  nops DELAY
  ld (hl), b
  dec a
  nops 105 - DELAY

line_end:
  dec e                ; 1
  jr nz, line_loop     ; 3
  nop

  ; 144:2
finish_frame:
  push bc
  long_delay 1133
  pop bc
  jp frame_start


tile:
.db %10000000
.db %10000000
.db %10000000
.db %10000000
.db %10000000
.db %10000000
.db %10000000
.db %10000000
.db %10000000
.db %10000000
.db %10000000
.db %10000000
.db %10000000
.db %10000000
.db %10000000
.db %10000000

.db %00010000
.db %00010000
.db %00111000
.db %00111000
.db %01111100
.db %01111100
.db %11111110
.db %11111110
.db %01111100
.db %01111100
.db %00111000
.db %00111000
.db %00010000
.db %00010000
.db %00000000
.db %00000000

.db %11111110
.db %11111110
.db %11111110
.db %11111110
.db %11111110
.db %11111110
.db %11111110
.db %11111110
.db %11111111
.db %11111110
.db %11111110
.db %11111110
.db %11111110
.db %11111110
.db %00000000
.db %00100000

.db %11111110
.db %11111110
.db %01111111
.db %01111111
.db %10111111
.db %10111111
.db %11011111
.db %11111111
.db %11101111
.db %11101111
.db %11110111
.db %11110111
.db %11111011
.db %11111011
.db %00000000
.db %00100000


