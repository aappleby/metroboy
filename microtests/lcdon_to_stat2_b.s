.include "header.inc"

main:
  reset_lcd_for_test

  nops 111

  test_finish_stat $82
