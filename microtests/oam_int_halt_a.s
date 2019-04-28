; pass - ags, dmg

.include "header.inc"

.ifdef DMG
.define DELAY 20
.else
.define DELAY 20
.endif

main:
  reset_lcd_for_test
  nops 114

  set_stat_int_oam
  set_ie_stat
  ei

  ldh (DIV), a
  halt
  nop


  test_fail

.org STAT_INT_VECTOR
  nops DELAY
  test_finish_div 1
