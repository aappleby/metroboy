.include "header.inc"

main:
  ld a, 7
  ldh (SCX), a

  cycle_init
  set_stat_int_hblank
  set_ie_stat
  clear_if
  ei

  xor a
  incs $31
  di
  test_fail

.org STAT_INT_VECTOR
  test_finish_a $31
  test_display_a