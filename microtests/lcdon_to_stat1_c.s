.include "header.inc"

main:
  reset_lcd_for_test

  long_delay 114*153

  nops 109

  test_finish_stat $85
