; pass - dmg

.include "header.inc"

main:
  lcd_off_unsafe
  lcd_on

  set_vblank_int
  clear_if

  ei

  ldh (DIV), a

  halt
  nop

  test_fail

.org VBLANK_INT_VECTOR
  nops 38
  test_finish_div 1
