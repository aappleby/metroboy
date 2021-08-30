; pass - dmg

.include "header.inc"

main:
  lcd_off_unsafe
  lcd_on

  set_ie_vblank
  clear_if

  ei

  ldh (DIV), a

  halt
  nop

  test_fail

.org VBLANK_INT_VECTOR
  nops 37
  test_finish_div 0
