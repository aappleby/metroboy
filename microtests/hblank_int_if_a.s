; pass - ags, dmg

.include "header.inc"

main:
  di

  lcd_off_unsafe
  lcd_on

  nops 114

  nops 45

  set_ie_stat
  clear_if
  set_stat_int_hblank

  ldh a, (IF)
  test_finish_a $E0