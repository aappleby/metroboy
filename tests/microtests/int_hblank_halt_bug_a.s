.include "header.inc"

main:
  ld a, %00001000
  ldh (STAT), a
  ld a, %00000010
  ldh (IE), a
  ld a, %00000000
  ldh (IF), a

  ld a, 0
  halt
  inc a

  test_finish_a 1

.org STAT_INT_VECTOR
  test_fail
