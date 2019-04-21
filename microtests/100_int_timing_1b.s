; tested - ags, dmg
; fails on metroboy if fasthalt = true
; fails on ags if halt replaced with nops

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
  nops 200

  test_fail

.org STAT_INT_VECTOR
  nops 25
  test_finish_div 2
