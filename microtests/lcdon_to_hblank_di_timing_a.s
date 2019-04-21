.include "header.inc"

; LCD on -> HBLANK int vs. DI timing

;-------------------------------------------------------------------------------

main:
  di
  clear_if

  lcd_off_unsafe
  lcd_on

  nops 114

  set_stat_int_hblank
  set_ie_stat
  ei

  nops 51

  di
  test_fail

.org STAT_INT_VECTOR
  test_pass

;-------------------------------------------------------------------------------
