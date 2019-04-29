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
  nop
  nops 55
  di
  test_finish_if $E0

.org STAT_INT_VECTOR
  test_pass
