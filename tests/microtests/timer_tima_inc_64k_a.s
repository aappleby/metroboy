.include "header.inc"

// 13 1
// 14 2
// 29 2
// 30 3

main:
  ld a, $00
  ldh (DIV), a
  ld a, $00
  ldh (TIMA), a
  ld a, $34
  ldh (TMA), a
  ld a, %00000110 // 65536 hz, one tick every 16 cycles
  ldh (TAC), a

  nops 13
  ldh a, (TIMA)
  test_finish_a 1