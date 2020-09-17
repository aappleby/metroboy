.include "header.inc"

main:
  lcd_off_unsafe
  set_stat_int_oam  
  clear_if

  lcd_on
  nops 109
  ldh a, (IF)

  test_finish_a $E0