.include "header.inc"

main:
  ld a, 0
  ldh (DIV), a
  set_ie_timer

  ld a, $E0
  ldh (TIMA), a

  ld a, %00000101
  ldh (TAC), a

  ei

  halt
  nop
  test_fail

.org TIMER_INT_VECTOR
  nops 43
  ldh a, (DIV)
  test_finish_a 3