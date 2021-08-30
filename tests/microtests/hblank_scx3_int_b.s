; pass - dmg

.include "header.inc"

main:
  reset_lcd_for_test

  ; line 0
  set_stat_int_hblank
  set_ie_stat
  ld a, 3
  ldh (SCX), a
  nops 98

  ; line 1
  clear_if
  ei
  nops 59
  di
  test_fail

.org STAT_INT_VECTOR
  test_pass
