; pass - dmg

.include "header.inc"

main:
  reset_lcd_for_test

  long_delay (152 * 114) + 106

  ld a,153
  ldh (<LYC),a

  test_finish_stat $85