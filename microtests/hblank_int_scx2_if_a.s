.include "header.inc"

main:
  nops 123

  ld a, 2
  ldh (SCX), a
  ld hl, $FF0F

  set_stat_int_hblank
  set_ie_stat
  clear_if
  ei

  xor a
  ldh (DIV), a

  nops 34
  ld a, (hl)
  di
  test_finish_a $E0

.org STAT_INT_VECTOR
  test_fail