.include "header.inc"

main:
  ld a, %00000101 // 262144 hz, one tick every 4 cycles
  ldh (TAC), a
  nops 0
  ldh a, (TIMA)
//- ld ($8000), a
//  jr -
  test_finish_a $00
