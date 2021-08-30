.include "header.inc"

;0->1 nada
;0->2 nada

; cycles 21 & 22 are weird

.define DELAY 20
main:
  lcd_off_unsafe

  ld a, %01110011
  ldh (LCDC), a

load_tile1:
  ld bc, tile_white
  ld hl, $8000
.repeat 16
  ld a, (bc)
  ld (hl+), a
  inc bc
.endr

load_tile2:
  ld bc, tile_black
  ld hl, $8000 + 16
.repeat 16
  ld a, (bc)
  ld (hl+), a
  inc bc
.endr

clear_map0:
  ld hl, $9800
  ld a, 0
  ld b, 0
- ld (hl+), a
  ld (hl+), a
  ld (hl+), a
  ld (hl+), a
  dec b
  jr nz, -

clear_map1:
  ld hl, $9C00
  ld a, 1
  ld b, 0
- ld (hl+), a
  ld (hl+), a
  ld (hl+), a
  ld (hl+), a
  dec b
  jr nz, -

  ld a, %11100100
  ldh (BGP), a

  ld a, %11110011
  ldh (LCDC), a        ; 2
  nops 2

  ; 000:18 - 110 cycles during line 0 to set up the frame
frame_start:
  ld hl, $FF4B         ; 3
  xor a                ; 1
  ld (hl), a           ; 2
  ld b, 0              ; 2
  ld c, 0              ; 2
  ld d, 0              ; 2
  ld e, 143            ; 2
  nops 110 - 14

  ; 001:2 - 110 cycles to mess with the line.
line_loop:
  ld (hl), b           ; 2
  inc b                ; 1

  nops 34              ; 30

  ld a, %11010011      ; 2
  ldh (LCDC), a        ; 3
  ld a, %11110011      ; 2
  ldh (LCDC), a        ; 3

  nops 110 - 47

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


tile_white:
.db %00000000
.db %00000000
.db %00000000
.db %00000000
.db %00000000
.db %00000000
.db %00000000
.db %00000000
.db %00000000
.db %00000000
.db %00000000
.db %00000000
.db %00000000
.db %00000000
.db %00000000
.db %00000000

tile_black:
.db %11111111
.db %11111111
.db %11111111
.db %11111111
.db %11111111
.db %11111111
.db %11111111
.db %11111111
.db %11111111
.db %11111111
.db %11111111
.db %11111111
.db %11111111
.db %11111111
.db %11111111
.db %11111111

tile1:
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

tile2:
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

tile3:
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

tile4:
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


