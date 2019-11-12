; pass - dmg

.include "header.inc"

main:
  set_ie_timer

  ld hl, 0

  ld a, %00000101
  ldh (TAC), a
  ld a, $FC
  ldh (DIV), a
  ldh (TIMA), a
  ldh (DIV), a
  ei

  nop
.repeat 20
  inc hl
.endr
  test_fail

.org TIMER_INT_VECTOR
  ld a, l
  test_finish_a 3
