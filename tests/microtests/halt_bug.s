.include "header.inc"

main:
  cycle_init

  ld a, 1
  ldh (IE), a
  ldh (IF), a
  halt
  //nop
  //nop
  inc a
  cycle_count
  test_finish_a 20
  //test_display_a

.org STAT_INT_VECTOR
  test_fail