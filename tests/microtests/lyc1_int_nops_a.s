; pass -

.include "header.inc"

main:
  reset_lcd_for_test

  ld a, 1
  ldh (LYC), a
  set_stat_int_lyc
  set_ie_stat
  ei

  ldh (DIV), a
  nops 200
  nop

  test_fail

.org STAT_INT_VECTOR
  nops 25
  test_finish_div 1