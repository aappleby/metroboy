; pass - ags, dmg

.include "header.inc"

; metroboy failing w/ 96

main:
  reset_lcd_for_test

  ; This delay loop will run out at the start of line 143
  xor a
  ld bc, 2972
- dec bc
  cp b
  jr nz, -

  xor a
  ldh (LYC), a
  set_stat_int_lyc
  set_ie_stat
  ei

  xor a
.repeat 1122
  inc a
.endr
  di
  test_fail

.org STAT_INT_VECTOR
  test_finish_a 98


