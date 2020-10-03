.include "header.inc"

main:
  lcd_off_unsafe
  ld a, 0
  ldh (SCX), a
  cycle_init
  set_stat_int_hblank
  set_ie_stat
  clear_if
  lcd_on
  ei         // 30
  xor a      // 31
  nops 60    // 91   // int fires @ phase 814, phase G of last op

  // 92
  // 93
  // 94
  // 95
  // 96

  di
  test_fail

.org STAT_INT_VECTOR
  test_finish_cycle 97