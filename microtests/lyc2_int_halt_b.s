; pass -

.include "header.inc"

; FIXME this is actually testing lyc2

main:
  reset_lcd_for_test
  nops 114

  ld a, 2
  ldh (LYC), a
  set_stat_int_lyc
  set_ie_stat
  ei

  ldh (DIV), a
  halt
  nop

  test_fail

.org STAT_INT_VECTOR
  nops 26
  test_finish_div 2