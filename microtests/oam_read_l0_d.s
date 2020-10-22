.include "header.inc"

main:
  ld hl, $FE00

  ld a, $0
  ldh (LCDC), a

  ld a, $F0
  ld (hl), a

  ld a, $91
  ldh (LCDC), a

  nops 61
  ld a, (hl)

  test_finish_a $F0