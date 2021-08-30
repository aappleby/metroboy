.include "header.inc"

// 0  = 1
// 1  = 1
// 2  = 2
// 3  = 2
// 4  = 2
// 5  = 2
// 6  = 3
// 7  = 3
// 8  = 3
// 9  = 3
// 10 = 4

main:
  ld a, $00
  ldh (DIV), a
  ld a, $00
  ldh (TIMA), a
  ld a, $34
  ldh (TMA), a
  ld a, %00000101 // 262144 hz, one tick every 4 cycles
  ldh (TAC), a

  nops 4
  ldh a, (TIMA)
  test_finish_a $02