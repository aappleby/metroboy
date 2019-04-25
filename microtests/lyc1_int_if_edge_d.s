; pass - dmg, ags

.include "header.inc"

main:
  reset_lcd_for_test

  ld a, 1
  ldh (LYC), a
  set_stat_int_lyc
  set_ie_stat

  nops 95

  clear_if
  test_finish_if $e0