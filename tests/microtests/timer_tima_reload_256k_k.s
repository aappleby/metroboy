.include "header.inc"

// 0  = FF
// 1  = FF
// 2  = 00
// 3  = 34
// 4  = 34
// 5  = 34
// 6  = 35
// 7  = 35
// 8  = 35
// 9  = 35
// 10 = 36

main:
  ld a, $00
  ldh (DIV), a
  ld a, $FE
  ldh (TIMA), a
  ld a, $34
  ldh (TMA), a
  ld a, %00000101 // 262144 hz, one tick every 4 cycles
  ldh (TAC), a

  nops 10
  ldh a, (TIMA)
  test_finish_a $36