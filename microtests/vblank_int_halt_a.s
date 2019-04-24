; pass - ags

.ifdef DMG
.define DELAY 27
.else
.define DELAY 26
.endif

.include "header.inc"

main:
  di
  lcd_off_unsafe
  lcd_on

  ; This delay loop will run out at the beginning of line 143
  xor a
  ld bc, 2972
- dec bc
  cp b
  jr nz, -

  set_stat_int_vblank
  set_ie_stat
  clear_if
  ei

  ldh (DIV), a
  halt
  nop

  test_fail

.org STAT_INT_VECTOR
  nops DELAY
  test_finish_div 1