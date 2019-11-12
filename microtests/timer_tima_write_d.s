; pass - dmg

.include "header.inc"

// 25 - 7F
// 26 - 80
// 27 - 80
// 28 - 7F
// 29 - FE
// 30 - 7F

test:
  ld hl, $FF05
  ld a, $fe
  ldh (TMA),a

  ld a, %00000110
  ldh (TAC), a

  ld a, $ff
  ldh (TIMA), a
  ldh (DIV),a

  nops 28

  ld a, $7f
  ld (hl),a
  ld a,(hl)
  test_finish_a $7F