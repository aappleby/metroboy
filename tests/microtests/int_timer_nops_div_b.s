.include "header.inc"

main:
  ld a, 0
  ldh (DIV), a
  set_ie_timer
  ld a, $E0
  ldh (TIMA), a
  ld a, %00000101
  ldh (TAC), a
  clear_if
  ei

  xor a
  nops 2000
  di
  test_fail

.org TIMER_INT_VECTOR
  nops 44
  test_finish_div 3