; pass - ags, dmg

.include "header.inc"

main:
  di
  clear_if

  lcd_off_unsafe
  lcd_on

  nops 114

  set_stat_int_oam
  set_ie_stat
  ei

  ldh (DIV), a
  halt
  nop


  test_fail

.org STAT_INT_VECTOR
  nops 20
  test_finish_div 1
