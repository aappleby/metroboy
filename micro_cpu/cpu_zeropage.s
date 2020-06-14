.include "header.inc"

main:
  ld a, 10
  ldh ($80), a
  ld a, 0
  ldh a,($80)
  jp main