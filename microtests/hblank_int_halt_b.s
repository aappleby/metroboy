.include "header.inc"

main:
  nops 41
  lcd_off_unsafe
  set_stat_int_hblank
  set_ie_stat
  clear_if
  ei

  lcd_on

  halt
  nop

  test_fail

.org STAT_INT_VECTOR
  ldh a, (DIV)
  test_finish_a $AE