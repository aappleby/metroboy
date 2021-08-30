; pass - ags

.include "header.inc"

main:
  di
  clear_stat
  clear_if
  lcd_off_unsafe
  lcd_on
  nops 114

  nops 108

  set_stat_int_oam
  ldh a, (IF)

  test_finish_a $E2