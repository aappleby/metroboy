.include "header.inc"

main:
  lcd_off_unsafe
  ld a, 7
  ldh (SCX), a
  cycle_init
  set_stat_int_hblank
  set_ie_stat
  clear_if
  lcd_on
  ei         // 30
  xor a      // 31
  nops 62    // 93

  // 94
  // 95
  // 96
  // 97
  // 98

  di
  test_fail

.org STAT_INT_VECTOR
  test_finish_cycle 99