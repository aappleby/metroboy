; pass - ags

.include "header.inc"

.ifdef DMG
.define DELAY 58
.else
.define DELAY 105
.endif

main:
  di
  clear_stat
  clear_if
  lcd_off_unsafe
  lcd_on
  nops 114

  nops DELAY

  set_stat_int_oam
  ldh a, (IF)

  test_finish_a $E2