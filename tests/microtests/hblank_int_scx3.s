; pass - dmg

.include "header.inc"

main:
  reset_lcd_for_test

  set_stat_int_hblank
  ld a, 3
  ldh (SCX), a

  nops 103

  set_ie_stat
  clear_if
  ei



  nops 58

  test_fail

.org STAT_INT_VECTOR
  test_pass
