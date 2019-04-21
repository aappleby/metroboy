.include "header.inc"

; LCD on -> OAM int while in an 'inc a' sled

;-------------------------------------------------------------------------------

main:

  lcd_off_unsafe
  set_stat_int_oam
  set_ie_stat
  clear_if
  ei

  lcd_on

  xor a
.repeat 200
  inc a
.endr

  test_fail

.org STAT_INT_VECTOR
  test_finish_a 111

;-------------------------------------------------------------------------------
