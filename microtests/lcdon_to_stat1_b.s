.include "header.inc"

main:
  reset_lcd_for_test

  long_delay 114*143

  nops 111

  test_finish_stat $81