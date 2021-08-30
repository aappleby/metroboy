.include "header.inc"

main:
  cycle_init

  set_stat_int_hblank
  set_ie_stat
  set_if

  xor a
  halt
  inc a

  test_finish_a 2

.org STAT_INT_VECTOR
  test_fail