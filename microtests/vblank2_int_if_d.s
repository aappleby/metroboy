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

  clear_stat
  nops 102

  clear_if
  ldh a, (IF)

  and $FD
  test_finish_a $E0