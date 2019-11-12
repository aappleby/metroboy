.include "header.inc"

main:
  ld a, %11111100
  ldh (TIMA), a
  ld a, %00001111
  ldh (TMA), a
  ld a, %00000101
  ldh (TAC), a

  xor a
  ldh a, (TMA)
  ldh a, (TAC)
  ldh a, (TIMA)
- jr -
