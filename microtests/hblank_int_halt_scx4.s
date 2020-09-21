.include "header.inc"

main:
  ld a, 4
  ldh (SCX), a

  cycle_init

  set_stat_int_hblank
  set_ie_stat
  clear_if
  ei

  halt
  test_fail

.org STAT_INT_VECTOR
  test_finish_cycle $4F