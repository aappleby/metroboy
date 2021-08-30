.include "header.inc"

main:
  lcd_off_unsafe
  ld a, 7
  ldh (SCX), a
  cycle_init
  set_stat_int_hblank
  set_ie_stat
  clear_if
  lcd_on
  ei
  xor a
  halt // int fires on 836 E







  di
  test_fail

.org STAT_INT_VECTOR
  test_finish_cycle 100