.include "header.inc"

main:

  lcd_off_unsafe
  set_stat_int_hblank
  set_ie_stat
  clear_if
  ei

  lcd_on
  incs 200
  test_fail

.org STAT_INT_VECTOR
  test_finish_a $CF
