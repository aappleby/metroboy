; pass on dmg
; passes on metroboy if fasthalt = true

.include "header.inc"

main:
  lcd_off_unsafe
  lcd_on

  set_stat_int_oam
  set_ie_stat
  clear_if
  ei

  ldh (DIV), a

  halt
  nop

  test_fail

.org STAT_INT_VECTOR
  nops 24
  test_finish_div 1
