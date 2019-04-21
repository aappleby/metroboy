; pass - ags

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

  set_vblank_int
  clear_if
  ei

  ldh (DIV), a
  nops 200


  test_fail

.org VBLANK_INT_VECTOR
  nops 23
  test_finish_div 2