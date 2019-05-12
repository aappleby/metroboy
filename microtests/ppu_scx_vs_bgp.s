.include "header.inc"

; cycles 19 & 20 are weird
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
  ld a, 0              ; 2
  ld b, 0              ; 2
  ld c, 0              ; 2
  ld d, 0              ; 2
  ld e, 143            ; 2
  ld hl, $FF43         ; 3
  nops 97

  ; 001:2 - 110 cycles to mess with the line.
line_loop:
  nops DELAY
  ld (hl), a            ; 2
  ld (hl), b            ; 2
  inc a
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
.db %10100010
.db %10001000
.db %10100010
.db %10001000
.db %10100010
.db %10001000
.db %10100010
.db %10001000
.db %10100010
.db %10001000
.db %10100010
.db %10001000
.db %10100010
.db %10001000
.db %10100010
.db %10001000
