; pass - ags, dmg

.include "header.inc"

main:
  di
  clear_stat
  clear_if
  lcd_off_unsafe
  lcd_on
  nops 114

  set_stat_int_oam
  set_ie_stat
  ei

  xor a
.repeat 200
  inc a
.endr

  test_fail

.org STAT_INT_VECTOR
  test_finish_a 100
