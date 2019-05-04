.include "header.inc"

; metroboy
; 7  - no move
; 8 - square moves
; 9 - no move

; 51 - no move
; 52 - move

; 101 - move
; 102 - no move

; dmg
; 4 - no move
; 5  - no move
; 6  - no moved
; 7  - right square (sprite 39) missing
; 8  - square moved
; 9  - no moved
; 10 - no moved

; 51 - no move
; 52 - move

; 101 - move
; 102 - move
; 103 - move
; 104 - move
; 105 - move
; 106 - move

.define DELAY 7

main:
  lcd_off_unsafe

  ; set sprite palette
  ld a, %11100100
  ldh (OBP0), a
  ldh (OBP1), a

  ; turn sprites on

  ; set tile to black
  ld hl, $8000h
  ld a, $ff
.repeat 16
  ld (hl+), a
.endr

  ; clear oam
  xor a
  ld b, 200
  ld hl, $FE00
clear_oam_loop:
  ld (hl+), a
  dec b
  jr nz, clear_oam_loop

  ; add sprites
  ld hl, $FE00 + 4 * 0
  ld a, 84
  ld (hl+), a
  ld a, 56
  ld (hl+), a
  ld a, 0
  ld (hl+), a
  ld a, 0
  ld (hl+), a

  ld hl, $FE00 + 4 * 39
  ld a, 84
  ld (hl+), a
  ld a, 56 + 8 + 48
  ld (hl+), a
  ld a, 0
  ld (hl+), a
  ld a, 0
  ld (hl+), a

  ; turn lcd on w/ sprites
  ld a, %10000011
  ldh (LCDC), a
  nops 114

frame_loop:
  ld a, 20
  ld b, 142
  ld hl, $FE00

line_loop:
  nops DELAY
  ld a, 70
  ld (hl), a
  nops 106 - DELAY

  dec b
  jr nz, line_loop

  long_delay 114 * 12 - 10
  jp frame_loop