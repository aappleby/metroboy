.include "header.inc"

main:
  lcd_off_unsafe
  cycle_init
  set_stat_int_lyc
  set_ie_stat
  clear_if
  ei
  ld a, 1
  ldh (LYC), a
  lcd_on

  xor a
  halt
  di
  test_fail

.org STAT_INT_VECTOR
  test_finish_cycle 153