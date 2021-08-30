.include "header.inc"

main:
  nops 123

  ld a, 6
  ldh (SCX), a
  ld hl, $FF0F

  set_stat_int_hblank
  set_ie_stat
  clear_if
  ei

  xor a
  ldh (DIV), a

  nops 37
  ld a, (hl)
  di
  test_fail

.org STAT_INT_VECTOR
  test_finish_a $E2