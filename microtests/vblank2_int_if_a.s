; pass - dmg

.include "header.inc"

main:
  reset_lcd_for_test

  long_delay 114 * 143

  nops 106

  clear_if
  test_finish_if $E0