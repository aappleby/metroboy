.include "header.inc"

.define DELAY 65
.define SPRITEX 0
.define SPRITEY 16
.define SCROLL 1
.define RESULT $80

.define FLAG_LCD_ON   %10000000
.define FLAG_WIN_MAP1 %01000000
.define FLAG_WIN_ON   %00100000
.define FLAG_TILE0    %00010000
.define FLAG_BG_MAP1  %00001000
.define FLAG_TALL_OBJ %00000100
.define FLAG_OBJ_ON   %00000010
.define FLAG_BG_ON    %00000001

main:
  ld a, 0
  ldh (LCDC), a

  ld a, 0
  ld b, 160
  ld hl, $FE00
- ld (hl+), a
  dec b
  jr nz, -

  ld hl, $FE00
  ld a, SPRITEY
  ld (hl+), a
  ld a, SPRITEX
  ld (hl+), a
  ld a, 0
  ld (hl+), a
  ld a, 0
  ld (hl+), a

  ld a, SCROLL
  ldh (SCX), a

  ld a, FLAG_LCD_ON | FLAG_BG_ON | FLAG_TILE0 | FLAG_OBJ_ON
  ldh (LCDC), a
  nops 114 - 2

  nops DELAY

  ldh a, (STAT)

  cp RESULT
  jr z, +
  ld a, $FF
- ld (VRAM), a
  jr -

+ ld a, $F0
- ld (VRAM), a
  jr -
