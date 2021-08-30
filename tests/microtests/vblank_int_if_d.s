; pass - ags, dmg

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
  nops 102

  clear_if
  ldh a, (IF)

  and $FE
  test_finish_a $E0