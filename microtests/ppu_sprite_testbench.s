.include "header.inc"

; .e - 253
; 8  - 265
; 9  - 
; 10 - 
; 11 - 
; 12 - 
; 13 - 
; 14 - 

.define SPRITEX 8

.define DELAY 20
main:
  lcd_off_unsafe

  ld a, %00000011
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

clear_map:
  ld hl, $9800
  ld a, 0
  ld b, 0
- ld (hl+), a
  ld (hl+), a
  ld (hl+), a
  ld (hl+), a
  ld (hl+), a
  ld (hl+), a
  ld (hl+), a
  ld (hl+), a
  dec b
  jr nz, -

load_sprites:
  ld a, 16
  ld ($FE00), a
  ld a, SPRITEX
  ld ($FE01), a
  ld a, 1
  ld ($FE02), a
  ld a, 0
  ld ($FE03), a

  ld a, %11100100
  ldh (BGP), a
  ldh (OBP0), a
  ldh (OBP1), a

  ld a, %10000011
  ldh (LCDC), a        ; 2
  nops 2

  ; 000:18 - 110 cycles during line 0 to set up the frame
frame_start:
  nops 110

  ; 001:2 - 110 cycles to mess with the line.
line_loop:
  nops 110

line_end:
  dec e                ; 1
  jr nz, line_loop     ; 3
  nop

  ; 144:2
finish_frame:
  push bc
  long_delay 1140 - 7
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


