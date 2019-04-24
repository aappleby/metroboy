; pass - ags, dmg

.ifdef DMG
.define DELAY 99
.else
.define DELAY 98
.endif

.include "header.inc"

main:
  di

  lcd_off_unsafe
  lcd_on

  nops 114

  nops DELAY

  set_ie_stat
  clear_if
  set_stat_int_hblank

  ldh a, (IF)
  test_finish_a $E2
