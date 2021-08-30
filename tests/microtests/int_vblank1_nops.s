.include "header.inc"

main:
  lcd_off_unsafe
  cycle_init
  set_stat_int_vblank
  set_ie_stat
  clear_if
  ei
  lcd_on

  long_delay 10000

  xor a
  nops 8000
  di
  test_fail

.org STAT_INT_VECTOR
  test_finish_cycle %01000010