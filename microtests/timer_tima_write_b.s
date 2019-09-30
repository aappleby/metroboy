; pass - dmg

.include "header.inc"

test:
  ld a, $fe
  ldh (TMA),a

  ld a, %00000110
  ldh (TAC), a

  ld a, $ff
  ldh (TIMA), a
  ldh (DIV),a

  nops 27

  ld a, $7f
  ldh (TIMA),a

  ldh a,(TIMA)
  test_finish_a $7F