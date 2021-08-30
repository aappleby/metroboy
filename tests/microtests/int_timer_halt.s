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
  halt



  di
  test_fail

.org TIMER_INT_VECTOR
  test_finish_cycle 15