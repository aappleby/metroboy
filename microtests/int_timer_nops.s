.include "header.inc"

main:
  ld a, 0
  ldh (TMA), a
  ldh (DIV), a
  set_ie_timer

  xor a
  ldh (IF), a

  ld a, $FE
  ldh (TIMA), a
  ld a, %00000101
  ldh (TAC), a
  ei    // 4
  nop   // 5
  nop   // 6
  nop   // 7
  nop   // 8

  // 9 - int fires on A
  // 10
  // 11
  // 12
  // 13

  di
  test_fail

.org TIMER_INT_VECTOR
  test_finish_cycle 14