.include "header.inc"

main:
  ld a, %00000110 // 64khz, one tick every 16 cycles
  ldh (TAC), a
  nops 0
  ldh a, (TIMA)
  test_finish_a $01
