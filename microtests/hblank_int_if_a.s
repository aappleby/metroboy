.include "header.inc"

main:

  lcd_off_unsafe
  set_stat_int_hblank

  clear_if


  lcd_on
  nops 59
  test_finish_if $E0