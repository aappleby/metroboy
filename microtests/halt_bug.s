.include "header.inc"

main:
  di
  ld a, 1
  ldh (IE), a
  ldh (IF), a
  halt
  inc a
  test_finish_a 3