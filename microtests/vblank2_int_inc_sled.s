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

.repeat 200
  inc a
.endr

  test_fail

.org VBLANK_INT_VECTOR
  test_finish_a 100