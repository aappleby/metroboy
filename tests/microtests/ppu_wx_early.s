.include "header.inc"

.define WINX 30

main:
  lcd_off_unsafe

  ld a, %11100100
  ldh (BGP), a

  load_tile 0 tile1
  load_tile 1 tile2

  clear_map0 0
  clear_map1 1

  ; turn lcd on
  ld a, %11110001
  ldh (LCDC), a        ; 2
  nops 2

  ; 000:18 - 110 cycles during line 0 to set up the frame
frame_start:
  ld e, 143            ; 2

  ld a, WINX           ; 2
  ldh (WX), a          ; 3
  ld a, 0              ; 2
  ldh (WY), a          ; 3
  nops 110 - 12

  ; 001:2 - 110 cycles to mess with the line.
line_loop:
  nops 20
  ld a, 200
  ldh (WX), a
  ld a, WINX
  ldh (WX), a

  nops 110 - 10 - 20

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


tile1:
.db %00000000
.db %11111111
.db %00000000
.db %11111111
.db %00000000
.db %11111111
.db %00000000
.db %11111111
.db %00000000
.db %11111111
.db %00000000
.db %11111111
.db %00000000
.db %11111111
.db %00000000
.db %11111111

tile2:
.db %11111111
.db %00000000
.db %11111111
.db %00000000
.db %11111111
.db %00000000
.db %11111111
.db %00000000
.db %11111111
.db %00000000
.db %11111111
.db %00000000
.db %11111111
.db %00000000
.db %11111111
.db %00000000
