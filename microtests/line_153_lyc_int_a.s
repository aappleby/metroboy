.include "header.inc"

; FIXME - need more thorough test

main:
  reset_lcd_for_test

  ld a,$f0
  ldh (LYC),a

  set_stat_int_lyc
  set_ie_stat
  clear_if
  ei

  long_delay 114 * 152
  
  nops 90
  
  ; this should happen on line 152, tcycle 455
  ; lyc write happens 153,5

  ld a,153
  ldh (LYC),a 
  nops 20

  di
  test_pass

.org STAT_INT_VECTOR
  test_fail

