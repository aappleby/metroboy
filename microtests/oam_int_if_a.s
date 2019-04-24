; pass - ags, dmg

.include "header.inc"

main:
  di
  lcd_off_unsafe
  lcd_on
  nops 114

  set_stat_int_oam
  nops 100

  clear_if
  ldh a, (IF)

  test_finish_a $E0