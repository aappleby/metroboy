; pass - dmg

.include "header.inc"

main:
  ld a, 0
  ldh (DIV), a
  nops 60
  ldh a, (DIV)
  test_finish_a $00
