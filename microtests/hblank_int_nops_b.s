; pass - ags, dmg

.ifdef DMG
.define DELAY 8
.else
.define DELAY 8
.endif

.include "header.inc"

main:
  reset_lcd_for_test
  nops 114

  set_stat_int_hblank
  set_ie_stat
  ei

  ldh (DIV), a
  nops 200



  test_fail

.org STAT_INT_VECTOR
  nops DELAY
  test_finish_div 1
