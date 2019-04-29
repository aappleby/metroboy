.include "header.inc"

; FIXME - need more thorough test

main:
  reset_lcd_for_test

  set_stat_int_lyc
  set_ie_stat

  long_delay 114 * 152

  clear_if
  ei
  
  nops 95
  
  ld a,$FF
  ldh (LYC),a 
  nops 20

  di
  test_pass

.org STAT_INT_VECTOR
  test_fail

