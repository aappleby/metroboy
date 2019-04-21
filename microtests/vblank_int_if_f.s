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

  nops 1236

  set_ie_stat
  clear_if
  set_stat_int_vblank

  ldh a, (IF)
  test_finish_a $E0