; pass - dmg

.include "header.inc"

main:
  set_ie_timer

  ld a, %00000101
  ldh (TAC), a
  ld a, $FC
  ldh (DIV), a
  ldh (TIMA), a
  ldh (DIV), a
  ei

  xor a
.repeat 20
  inc a
.endr
  test_fail

.org TIMER_INT_VECTOR
  test_finish_a 6
