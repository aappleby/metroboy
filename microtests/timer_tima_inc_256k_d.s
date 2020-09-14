.include "header.inc"

main:
  ld a, %00000101 // 262144 hz, one tick every 4 cycles
  ldh (TAC), a
  nops 5
  ldh a, (TIMA)
  test_finish_a $02
