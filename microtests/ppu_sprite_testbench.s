.include "header.inc"

.define DELAY 20
main:
  lcd_off_unsafe

  load_tile 0 tile_white
  load_tile 1 tile_black

  memset_1024 $9800 0
  memset_1024 $9C00 1

  load_sprite 0 16 0 1 0
  load_sprite 1 16 0 1 0

  ld a, %11100100
  ldh (BGP), a
  ldh (OBP0), a
  ldh (OBP1), a

  lcd_on_sprites
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

tile_diamond:
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

tile_leftedge:
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

tile_box:
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

tile_tiltbox:
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
