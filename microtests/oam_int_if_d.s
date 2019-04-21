; pass - ags

.include "header.inc"

; 95 - off
; 96 - on
; 99 - on
; 100 - off

main:
  di

  lcd_off_unsafe
  lcd_on

  nops 100

  set_ie_stat
  clear_if
  set_stat_int_oam

  ldh a, (IF)
  test_finish_a $E0