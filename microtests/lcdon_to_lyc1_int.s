.include "header.inc"

; LCD on -> HBLANK int

;-------------------------------------------------------------------------------

main:

  ld a, 1
  ldh (LYC), a

  lcd_off_unsafe
  set_stat_int_lyc
  set_ie_stat
  clear_if
  ei

  lcd_on

  xor a
.repeat 1000
  inc a
.endr

  test_fail

.org STAT_INT_VECTOR
  test_finish_a 112

;-------------------------------------------------------------------------------
