.include "header.inc"

; ags - 19 & 20
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
  ld a, 0              ; 2
  ld b, 0              ; 2
  ld c, 0              ; 2
  ld d, 0              ; 2
  ld e, 143            ; 2
  ld hl, $FF43         ; 3
  nops 99              ; 99

  ; this loop should start at line 1, tcycle 2
line_loop:
  nops DELAY
  ld (hl), a            ; 2
  ld (hl), b            ; 2
  inc a

  nops 105 - DELAY

  ;ld a, d              ; 1
  ;ldh (SCX), a         ; 3
  ;inc a                ; 1
  ;ld d, a              ; 1
  ;nops 18
  ;ld a, %11111111      ; 2
  ;ldh (BGP), a         ; 3
  ;ld a, %11100100      ; 2
  ;ldh (BGP), a         ; 3
  ;ld a, 0              ; 2
  ;ldh (SCX), a         ; 3
  ;nops 110 - 18 - 21

  ;nops DELAY           ; 30    30
  ;ld a, %11111111      ;  2    32
  ;ld (HL), a           ;  2    34
  ;ld a, %11100100      ;  2    36
  ;ld (HL), a           ;  2    38
  ;nops 102 - DELAY     ; 72   110

line_end:
  dec e                ; 1
  jr nz, line_loop     ; 3
  nop

finish_frame:
  ; 144:2
  long_delay 1246      ; 1246
  ld d, 0              ; 2
  ld e, 143            ; 2
  jp line_loop         ; 4
  ; 1254 = 114 * 11


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


.db %00001111
.db %00000000
.db %00001111
.db %00000000
.db %00001111
.db %00000000
.db %00001111
.db %00000000

.db %00001111
.db %11111111
.db %00001111
.db %11111111
.db %00001111
.db %11111111
.db %00001111
.db %11111111


.db %00000000
.db %00000000

.db %11111111
.db %00000000

.db %00000000
.db %11111111

.db %11111111
.db %11111111
.db %00000000
.db %00000000

.db %11111111
.db %00000000

.db %00000000
.db %11111111

.db %11111111
.db %11111111
