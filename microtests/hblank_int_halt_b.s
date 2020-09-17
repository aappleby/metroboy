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
  test_fail

.org STAT_INT_VECTOR
  test_finish_div $AE
  